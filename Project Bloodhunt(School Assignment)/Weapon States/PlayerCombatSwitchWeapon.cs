using System.Collections;
using UnityEngine;
using UnityEngine.InputSystem;
using SoulsLikeProject.Inventory;

namespace SoulsLikeProject.Combat
{
    public class PlayerCombatSwitchWeapon : MonoBehaviour
    {
        [Header("Attack References")]
        [SerializeField] PlayerCombatAttackHandler attackHandler;

        [Header("Starting Weapon")]
        [SerializeField] public WeaponData startingWeapon;

        [Header("Weapon Hand Slot")]
        [SerializeField] Transform weaponSlot;
        [SerializeField] Transform heavyWeaponSlot;

        [Header("Animator & Layer Handler")]
        [SerializeField] Animator animator;
        [SerializeField] AnimationLayerHandler animLayer;

        [Header("Stash Weapon Prefab")]
        [SerializeField] PlayerStashWeapon stashWeaponPrefab;

        [Header("State")]
        public WeaponData equippedWeapon;
        public bool isUnarmed = true;
        public bool currentlySwitchingWeapon = false;
        public bool startUnarmed = true;

        private WeaponData queuedWeapon = null;
        private GameObject currentWeapon;
        

        void Start()
        {
            if (animator == null) animator = GetComponent<Animator>();
            if (animLayer == null) animLayer = GetComponent<AnimationLayerHandler>();
            if (attackHandler == null) attackHandler = GetComponent<PlayerCombatAttackHandler>();

            isUnarmed = true;
            equippedWeapon = null;

            if (currentWeapon != null) { Destroy(currentWeapon); currentWeapon = null; }
            if (attackHandler != null) { attackHandler.lightAttacks = null; attackHandler.heavyAttacks = null; }
            if (animLayer != null) animLayer.RevertAfterSwitch(true);

        }

        #region Input

        public void OnEquipWeapon(InputAction.CallbackContext context)
        {
            if (!context.performed) return;
            if (currentlySwitchingWeapon) return;

            if(attackHandler!= null)
            {
                attackHandler.ResetAttackState();
            }

            currentlySwitchingWeapon = true;

            if (animLayer != null)
            {
                animLayer.RequestSwitching();
            }

            if (isUnarmed)
            {
                queuedWeapon = GetWeaponToEquip();
                if (animator != null)
                {
                    animator.ResetTrigger("WeaponToUnarmed");
                    animator.SetTrigger("UnarmedToWeapon");
                }
            }
            else
            {
                if (animator != null)
                {
                    animator.ResetTrigger("UnarmedToWeapon");
                    animator.SetTrigger("WeaponToUnarmed");
                }
            }

        }

        #endregion

        #region Equip / Unequip

        public void EquipByReference(WeaponData weapon)
        {
            if (currentlySwitchingWeapon)
            {
                return;
            }

            if (weapon == null)
            {
                Unequip();
                return;
            }

            Equip(weapon);
            isUnarmed = false;
        }

        public void EquipFromQuest(WeaponData chosenWeapon)
        {
            if (chosenWeapon == null) return;

            queuedWeapon = chosenWeapon;

            Equip(chosenWeapon);
            if (animLayer != null) animLayer.RevertAfterSwitch(false);
        }

        public void Equip(WeaponData weapon)
        {
            equippedWeapon = weapon;
            if (weapon == null)
            {
                return;
            }

            ReplaceWeapon(weapon.prefab);

            if (attackHandler != null)
            {
                attackHandler.lightAttacks = weapon.lightAttacks;
                attackHandler.heavyAttacks = weapon.heavyAttacks;
            }

            isUnarmed = false;
        }

        public void Unequip()
        {
            if (currentWeapon != null) Destroy(currentWeapon);
            equippedWeapon = null;

            if (attackHandler != null)
            {
                attackHandler.lightAttacks = null;
                attackHandler.heavyAttacks = null;
            }

            isUnarmed = true;
        }

        /// <summary>
        /// Resolves weapon from hub: DamageWeapon item's linkedWeaponData if any, else startingWeapon.
        /// </summary>
        public WeaponData GetWeaponToEquip()
        {
            if (HubInventoryManager.Instance == null) return startingWeapon;

            foreach (var inv in HubInventoryManager.Instance.Items)
            {
                if (inv?.itemData == null) continue;
                if (inv.itemData.exclusiveGroup != "DamageWeapon") continue;
                if (inv.itemData.linkedWeaponData != null)
                    return inv.itemData.linkedWeaponData;
                break;
            }

            return startingWeapon;
        }

        /// <summary>
        /// Re-equip from hub (e.g. after crafting a weapon). If armed, swaps to current DamageWeapon or startingWeapon.
        /// </summary>
        public void RefreshEquippedWeaponFromHub()
        {
            if (currentlySwitchingWeapon) return;
            if (isUnarmed) return;

            WeaponData next = GetWeaponToEquip();
            if (next != null)
                Equip(next);
        }

        #endregion

        #region Weapon Instantiation

        private void ReplaceWeapon(GameObject prefab)
        {
            if (currentWeapon != null) Destroy(currentWeapon);
            if (prefab == null) return;

            if(!equippedWeapon.isHeavy)
            {
                currentWeapon = Instantiate(prefab, weaponSlot);
                PlayerCombatSwordTip newSwordTip = currentWeapon.GetComponentInChildren<PlayerCombatSwordTip>();
                if (newSwordTip != null && attackHandler != null)
                {
                    attackHandler.SetSwordTip(newSwordTip);
                    var weaponComponent = currentWeapon.GetComponent<Weapon>();
                    if (weaponComponent != null && weaponComponent.data != null)
                        newSwordTip.SetWeaponData(weaponComponent.data);
                    else
                        newSwordTip.SetWeaponData(null);
                }
            }
            else
            {
                currentWeapon = Instantiate(prefab, heavyWeaponSlot);
                PlayerCombatSwordTip newSwordTip = currentWeapon.GetComponentInChildren<PlayerCombatSwordTip>();
                if (newSwordTip != null && attackHandler != null)
                {
                    attackHandler.SetSwordTip(newSwordTip);
                    var weaponComponent = currentWeapon.GetComponent<Weapon>();
                    if (weaponComponent != null && weaponComponent.data != null)
                        newSwordTip.SetWeaponData(weaponComponent.data);
                    else
                        newSwordTip.SetWeaponData(null);
                }
            }

        }

        #endregion

        #region Animation Events

        public void AnimOnStash()
        {
            queuedWeapon = null;

            if (currentWeapon != null) Destroy(currentWeapon);
            
            if (animLayer != null) animLayer.RevertAfterSwitch(true);

            isUnarmed = true;
            currentlySwitchingWeapon = false;
            stashWeaponPrefab.StashWeaponPrefab();
        }

        public void AnimOnEquip()
        {
            var toEquip = queuedWeapon != null ? queuedWeapon : GetWeaponToEquip();

            if (toEquip != null)
            {
                Equip(toEquip);
            }

            queuedWeapon = null;

            animLayer.RevertAllLayers();

            stashWeaponPrefab.UnstashWeaponPrefab();

            currentlySwitchingWeapon = false;
        }

        #endregion
    }
}