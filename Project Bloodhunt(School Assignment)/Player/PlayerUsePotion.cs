using UnityEngine;
using UnityEngine.Events;
using UnityEngine.InputSystem;
using SoulsLikeProject.Inventory;
using SoulsLikeProject.Player.Temp;
using SoulsLikeProject.Player;
using SoulsLikeProject.Combat;
using System.Collections;

namespace SoulsLikeProject.Inventory
{
    public class PlayerUsePotion : MonoBehaviour
    {
        [Header("Potion Settings")]
        [SerializeField] private int maxPotionSlots = 4;
        [SerializeField] private KeyCode drinkPotionkey = KeyCode.Space;

        [Header("UI Reference")]
        [SerializeField] private PotionSlotUI[] potionSlots = new PotionSlotUI[4];

        [SerializeField] private UnityEvent<ItemData> onPotionUsed = new UnityEvent<ItemData>();

        [Header("Player References")]
        [SerializeField] private PlayerHealth playerHealth;
        [SerializeField] private PlayerStamina playerStamina;
        [SerializeField] private PlayerCombatAttackHandler attackHandler;
        [SerializeField] private PlayerActionState actionState;
        [SerializeField] private Animator animator;

        [Header("FX Prefabs")]
        [SerializeField] GameObject staminaBuff;
        [SerializeField] GameObject strengthBuff;
        [SerializeField] GameObject healBuff;

        [Header("Currently Using Potion")]
        public bool currentlyUsingPotion = false;

        private int currentSelectedSlot = 0;
        private Keyboard keyboard;
        private bool hasInitialized = false;

        private float potionEffectDelay = 2f;
        private Coroutine currentDamageBoostCoroutine;
        private Coroutine currentStaminaBoostCoroutine;
        private float originalDamageMultiplier = 1f;
        private float originalStaminaRegenRate;

        private void Start()
        {
            if (playerStamina != null)
            {
                originalStaminaRegenRate = playerStamina.StaminaRegenRate;
            }
            
            if (attackHandler != null)
            {
                originalDamageMultiplier = attackHandler.GetDamageMultiplier();
            }

            if (staminaBuff != null)
            {
                staminaBuff.SetActive(false);
            }
            if (strengthBuff != null)
            {
                strengthBuff.SetActive(false);
            }
            if (healBuff != null)
            {
                healBuff.SetActive(false);
            }

            currentlyUsingPotion = false;

            Invoke(nameof(InitializeUI), 0.01f);
        }

        private void InitializeUI()
        {
            if (!hasInitialized)
            {
                InitializePotionSlots();
                SelectSlot(0);
                hasInitialized = true;
            }
        }

        private void Update()
        {
            if (keyboard == null)
            {
                keyboard = Keyboard.current;
            }
            
            if (keyboard == null) return;
            
            HandleSlotSelection();
            HandlePotionUsage();
        }

        private void InitializePotionSlots()
        {
            var questItems = QuestInventoryManager.Instance.QuestItems;
            int slotIndex = 0;

            for (int i = 0; i < potionSlots.Length; i++)
            {
                if (potionSlots[i] != null)
                {
                    potionSlots[i].gameObject.SetActive(false);
                }
            }
            
            foreach (var questItem in questItems)
            {
                if (slotIndex >= maxPotionSlots) break;
                if (questItem.itemData == null || !questItem.itemData.isUsable) continue;

                if (potionSlots[slotIndex] != null)
                {
                    potionSlots[slotIndex].SetupSlot(questItem.itemData);
                    potionSlots[slotIndex].gameObject.SetActive(true);
                }

                slotIndex++;
            }
        }

        private void HandleSlotSelection()
        {
            if (keyboard.digit1Key.wasPressedThisFrame)
            {
                SelectSlot(0);
            }
            else if (keyboard.digit2Key.wasPressedThisFrame)
            {
                SelectSlot(1);
            }
            else if (keyboard.digit3Key.wasPressedThisFrame)
            {
                SelectSlot(2);
            }
            else if (keyboard.digit4Key.wasPressedThisFrame)
            {
                SelectSlot(3);
            }
        }

        private void SelectSlot(int slotIndex)
        {
            if (slotIndex < 0 || slotIndex >= maxPotionSlots) return;
            if (potionSlots[slotIndex] == null || !potionSlots[slotIndex].gameObject.activeSelf) return;

            if (potionSlots[currentSelectedSlot] != null)
            {
                potionSlots[currentSelectedSlot].SetSelected(false);
            }

            currentSelectedSlot = slotIndex;
            potionSlots[currentSelectedSlot].SetSelected(true);
        }

        private void HandlePotionUsage()
        {
            if (keyboard.spaceKey.wasPressedThisFrame)
            {
                UseSelectedPotion();
            }
        }

        private void UseSelectedPotion()
        {
            PotionSlotUI selectedSlot = potionSlots[currentSelectedSlot];
            ItemData potionData = selectedSlot.GetItemData();

            if (potionData == null) return;
            if (actionState == null || animator == null) return;
            if (!actionState.CanDoState) return;

            bool removed = QuestInventoryManager.Instance.RemoveItem(potionData, 1);
            if (!removed)
            {
                actionState.ForceIdle();
                return;
            }

            ApplyPotionEffects(potionData);

            int newQuantity = QuestInventoryManager.Instance.GetItemQuantity(potionData);
            selectedSlot.UpdateQuantity(newQuantity);
        }

        private void ApplyPotionEffects(ItemData potion)
        {
            if (potion == null) return;

            currentlyUsingPotion = true;

            animator.SetTrigger("UsePotion");

            if (potion.healthRestoreAmount > 0 && playerHealth != null)
            {
                playerHealth.Heal(potion.healthRestoreAmount);
                if (healBuff != null)
                {
                    healBuff.SetActive(true);
                }
            }

            if (potion.staminaRegenScale > 0 && potion.staminaEffectDuration > 0 && playerStamina != null)
            {
                if (currentStaminaBoostCoroutine != null)
                {
                    if (staminaBuff != null)
                    {
                        staminaBuff.SetActive(true);
                    }
                    StopCoroutine(currentStaminaBoostCoroutine);
                }
                currentStaminaBoostCoroutine = StartCoroutine(ApplyStaminaBoost(potion));
            }

            if (potion.strengthIncreseAmount > 0 && potion.strengthEffectDuration > 0 && attackHandler != null)
            {
                if (currentDamageBoostCoroutine != null)
                {
                    if (strengthBuff != null)
                    {
                        strengthBuff.SetActive(true);
                    }

                    StopCoroutine(currentDamageBoostCoroutine);
                }
                currentDamageBoostCoroutine = StartCoroutine(ApplyDamageBoost(potion));
            }
        }

        private IEnumerator ApplyStaminaBoost(ItemData potion)
        {
            float boostedRegenRate = originalStaminaRegenRate * potion.staminaRegenScale;
            playerStamina.StaminaRegenRate = boostedRegenRate;
            
            yield return new WaitForSeconds(potion.staminaEffectDuration);
            
            playerStamina.StaminaRegenRate = originalStaminaRegenRate;
            currentStaminaBoostCoroutine = null;
        }

        private IEnumerator ApplyDamageBoost(ItemData potion)
        {
            float boostedDamage = originalDamageMultiplier * (1f + potion.strengthIncreseAmount);
            attackHandler.SetDamageMultiplier(boostedDamage);
            
            yield return new WaitForSeconds(potion.strengthEffectDuration);
            
            attackHandler.SetDamageMultiplier(originalDamageMultiplier);
            currentDamageBoostCoroutine = null;
        }

        public void RefreshPotionUI()
        {
            InitializePotionSlots();
            SelectSlot(0);
        }

        public ItemData GetSelectedPotion()
        {
            return potionSlots[currentSelectedSlot].GetItemData();
        }

        public void PotionListener(UnityAction<ItemData> listener)
        {
            onPotionUsed.AddListener(listener);
        }

        public void RemovePotionListener(UnityAction<ItemData> listener)
        {
            onPotionUsed.RemoveListener(listener);
        }

        public void OnPotionAnimationComplete()
        {
            if (actionState != null)
            {
                actionState.OnActionComplete();
            }
            currentlyUsingPotion = false;
            staminaBuff.SetActive(false);
            strengthBuff.SetActive(false);
            healBuff.SetActive(false);
        }
    }
}

