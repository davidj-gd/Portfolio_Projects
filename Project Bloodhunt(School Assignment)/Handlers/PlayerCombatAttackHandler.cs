using SoulsLikeProject.Combat.Interfaces;
using SoulsLikeProject.Player;
using SoulsLikeProject.Player.Temp;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;

namespace SoulsLikeProject.Combat
{
    public class PlayerCombatAttackHandler : MonoBehaviour
    {
        #region Fields and Properties

        [Header("Attack Data")]
        public PlayerCombatAttackData[] lightAttacks;
        public PlayerCombatAttackData[] heavyAttacks;

        [Header("Player Reference")]
        [SerializeField] Animator animator;
        [SerializeField] PlayerInput playerInput;
        [SerializeField] PlayerMovement playerMovement;
        [SerializeField] PlayerDash playerDash;
        [SerializeField] PlayerCombatParry playerParry;

        [Header("Action State")]
        [SerializeField] PlayerActionState actionState;

        [Header("Sword Reference")]
        [SerializeField] private PlayerCombatSwordTip swordTip;

        [Header("Handlers")]
        [SerializeField] private MonoBehaviour lightHandlerBehaviour;
        [SerializeField] private MonoBehaviour heavyHandlerBehaviour;

        [Header("Stamina Reference")]
        [SerializeField] private PlayerStamina playerStamina;

        [Header("Switch Weapon Reference")]
        [SerializeField] PlayerCombatSwitchWeapon switchWeapon;

        [Header("Player Interact")]
        [SerializeField] PlayerInteract playerInteract;

        [Header("Attack Type Handler")]
        private IPlayerCombatHandler lightHandler;
        private IPlayerCombatHandler heavyHandler;

        public enum ComboType { None, Light, Heavy };
        public enum QueuedAttackType { None, Light, Heavy };
        public ComboType currentCombo = ComboType.None;
        public QueuedAttackType queuedAttack = QueuedAttackType.None;

        public int lightIndex = 0;
        public int heavyIndex = 0;

        public bool isAttacking = false;
        public int currentAttackIndex = 0;
        public bool queueNextAttack = false;
        public bool canCombo = false;
        public bool attackFinished = false;

        private InputAction attackAction;
        private InputAction heavyAction;

        private float attackResetTime = 2.5f;
        private Coroutine currentAttackCoroutine = null;

        private float damageMultiplier = 1f;


        #endregion

        #region Unity Lifecycle

        private void Awake()
        {
            lightHandler = lightHandlerBehaviour as IPlayerCombatHandler;
            heavyHandler = heavyHandlerBehaviour as IPlayerCombatHandler;

            playerInput = GetComponent<PlayerInput>();
                
        }

        #endregion

        #region Enable & Disbale Attack Inputs
        private void OnEnable()
        {
            if (playerInput != null && playerInput.actions != null)
            {
                attackAction = playerInput.actions["Attack"];
                attackAction.performed += OnAttack;

                heavyAction = playerInput.actions["HeavyAttack"];
                heavyAction.performed += OnHeavyAttack;
            }
            else
            {
                Debug.LogWarning("PlayerInput or actions not found in PlayerCombatAttackHandler.");
            }
        }

        private void OnDisable()
        {
            if (attackAction != null)
            {
                attackAction.performed -= OnAttack;
            }
            else
            {
                Debug.LogWarning("Attack action not found in OnDisable.");
            }

            if (heavyAction != null)
            {
                heavyAction.performed -= OnHeavyAttack;
            }
            else
            {
                 Debug.LogWarning("Heavy attack action not found in OnDisable.");
            }
        }
        #endregion

        #region Input Actions

        public void OnAttack(InputAction.CallbackContext context)
        {
            if (!context.performed) return;

            if (playerInteract.isInteracting) return;

            if (switchWeapon == null || switchWeapon.equippedWeapon == null) return;

            if (switchWeapon.isUnarmed) return;

            if (actionState != null && !actionState.CanDoState)
            {
                if (actionState.IsAttacking && canCombo && !queueNextAttack)
                {
                    queueNextAttack = true;
                    queuedAttack = QueuedAttackType.Light;
                }

                return;
            }

            if (playerStamina == null)
            {
                Debug.LogWarning("PlayerStamina is not assigned! Attack cannot use stamina.", this);
                TryPerformLightAttack(lightHandler);
                return;
            }

            if (playerStamina.UseStamina(7f))
            {
                TryPerformLightAttack(lightHandler);
            }
        }

        public void OnHeavyAttack(InputAction.CallbackContext context)
        {
            if (!context.performed) return;

            if (switchWeapon == null || switchWeapon.equippedWeapon == null) return;

            if (switchWeapon.isUnarmed) return;

            if (actionState != null && !actionState.CanDoState)
            {
                if (actionState.IsAttacking && canCombo && !queueNextAttack)
                {
                    queueNextAttack = true;
                    queuedAttack = QueuedAttackType.Heavy;
                }

                return;
            }

            if (playerStamina == null)
            {
                TryPerformHeavyAttack(heavyHandler);
                return;
            }

            if (playerStamina.UseStamina(15f))
            {
                TryPerformHeavyAttack(heavyHandler);
            }
        }

        #endregion

        #region Light & Heavy Attack Methods
        private void TryPerformLightAttack(IPlayerCombatHandler handler)
        {

            PlayerCombatAttackData[] availableLight = null;

            if (switchWeapon != null && switchWeapon.equippedWeapon != null)
                availableLight = switchWeapon.equippedWeapon.lightAttacks;

            if (availableLight == null || availableLight.Length == 0)
                availableLight = lightAttacks; 

            if (availableLight == null || availableLight.Length == 0)
            {
                return;
            }

            if (isAttacking) return;

            if (actionState != null)
            {
                if(!actionState.TrySetState(PlayerActionState.ActionState.Attacking))
                {
                    return;
                }
            }

            if (currentAttackIndex < 0 || currentAttackIndex >= availableLight.Length)
            {
                currentAttackIndex = 0;
            }

            PlayerCombatAttackData attack = availableLight[currentAttackIndex];

            if (attack == null)
            {
                return;
            }

            if (handler == null)
            {
                return;
            }

            var idx = currentAttackIndex;
            StartCoroutine(ExecuteAttack(handler, attack, idx));
        }

        private void TryPerformHeavyAttack(IPlayerCombatHandler handler)
        {
            PlayerCombatAttackData[] availableHeavy = null;

            if (switchWeapon != null && switchWeapon.equippedWeapon != null)
                availableHeavy = switchWeapon.equippedWeapon.heavyAttacks;

            if (availableHeavy == null || availableHeavy.Length == 0)
                availableHeavy = heavyAttacks; 

            if (availableHeavy == null || availableHeavy.Length == 0)
            {
                return;
            }

            if (isAttacking) return;

            if (actionState != null)
            {
                if (!actionState.TrySetState(PlayerActionState.ActionState.Attacking))
                {
                    return;
                }
            }

            int heavyAttackIndex = 0;

            if (heavyAttackIndex < 0 || heavyAttackIndex >= availableHeavy.Length) return;

            PlayerCombatAttackData attack = availableHeavy[heavyAttackIndex];

            if (attack == null)
            {
                return;
            }

            if (handler == null)
            {
                return;
            }

            StartCoroutine(ExecuteAttack(handler, attack, 0));
        }
        #endregion

        #region Coroutine Attack Execution
        public IEnumerator ExecuteAttack(IPlayerCombatHandler handler, PlayerCombatAttackData attack, int comboIndex)
        {
            if (attack == null || handler == null)
            {
                yield break;
            }

            if (animator == null)
            {
                yield break;
            }

            isAttacking = true;
            canCombo = false;

            GetWeaponFX()?.PlayAttackVFX();

            yield return StartCoroutine(handler.PerformAttack(attack, swordTip, animator));

        }

        private IEnumerator AttackTimeout()
        {
            yield return new WaitForSeconds(attackResetTime);

            if (isAttacking)
            {
                AnimatorStateInfo stateInfo = animator.GetCurrentAnimatorStateInfo(0);

                bool isInAttackState = stateInfo.IsTag("Attack");

                if (!isInAttackState && isAttacking)
                {
                    ResetAttackState();
                }
            }
            
        }

        #endregion

        #region Sword Tip Management

        public void SetSwordTip(PlayerCombatSwordTip newSwordTip)
        {
            if (newSwordTip == null)
            {
                return;
            }

            swordTip = newSwordTip;
        }

        private WeaponFX GetWeaponFX()
        {
            if (swordTip == null) return null;
            var weapon = swordTip.GetComponentInParent<Weapon>();
            return weapon != null ? weapon.GetComponentInChildren<WeaponFX>() : swordTip.GetComponentInChildren<WeaponFX>();
        }

        #endregion

        #region Animation Event Methods
        public void EnableCombo()
        {
            canCombo = true;
        }
        public void DisableCombo()
        {
            canCombo = false;
            GetWeaponFX()?.StopAttackVFX();
        }

        public void AttackEnded()
        {
            
            attackFinished = true;

            if (animator != null)
            {
                animator.SetBool("isAttacking", false);
            }

            if (!queueNextAttack)
            {
                queuedAttack = QueuedAttackType.None;
                isAttacking = false;
                currentCombo = ComboType.None;
                lightIndex = 0;
                heavyIndex = 0;
                currentAttackIndex = 0;
                playerMovement.OnAttackEnd();

                if(actionState != null)
                {
                    actionState.TrySetState(PlayerActionState.ActionState.Idle);
                }

                return;
            }

            queueNextAttack = false;
            isAttacking = false;

            if (queuedAttack == QueuedAttackType.Light)
            {
                attackFinished = false;
                QueuedAttackType savedQueue = queuedAttack;
                queuedAttack = QueuedAttackType.None;

                PlayerCombatAttackData[] availableLight = null;
                if (switchWeapon != null && switchWeapon.equippedWeapon != null)
                    availableLight = switchWeapon.equippedWeapon.lightAttacks;
                if (availableLight == null || availableLight.Length == 0)
                    availableLight = lightAttacks;

                if (availableLight != null && availableLight.Length > 0)
                {
                    currentAttackIndex = (currentAttackIndex + 1) % availableLight.Length;

                    PlayerCombatAttackData attack = availableLight[currentAttackIndex];
                    if (attack != null && lightHandler != null)
                    {
                        StartCoroutine(ExecuteAttack(lightHandler, attack, currentAttackIndex));
                        return;
                    }
                }
            }
            else if (queuedAttack == QueuedAttackType.Heavy)
            {
                attackFinished = false;
                QueuedAttackType savedQueue = queuedAttack;
                queuedAttack = QueuedAttackType.None;

                PlayerCombatAttackData[] availableHeavy = null;
                if (switchWeapon != null && switchWeapon.equippedWeapon != null)
                    availableHeavy = switchWeapon.equippedWeapon.heavyAttacks;
                if (availableHeavy == null || availableHeavy.Length == 0)
                    availableHeavy = heavyAttacks;

                if (availableHeavy != null && availableHeavy.Length > 0)
                {
                    int heavyAttackIndex = 0;
                    PlayerCombatAttackData attack = availableHeavy[heavyAttackIndex];
                    if (attack != null && heavyHandler != null)
                    {
                        StartCoroutine(ExecuteAttack(heavyHandler, attack, heavyAttackIndex));
                        return;
                    }
                }
            }

            currentCombo = ComboType.None;
            lightIndex = 0;
            heavyIndex = 0;
            currentAttackIndex = 0;
            playerMovement.OnAttackEnd();
        }

        #endregion

        #region Combo Helpers

        public void RegisterLightAttack(Animator animator)
        {
            actionState.TrySetState(PlayerActionState.ActionState.Attacking);

            if (currentCombo != ComboType.Light)
            {
                lightIndex = 0;
                currentCombo = ComboType.Light;
            }

            currentAttackIndex = lightIndex;

            if (!switchWeapon.equippedWeapon.isHeavy)
            {
                animator.SetTrigger($"LightAttack_{lightIndex + 1}");
            }
            else
            {
                animator.SetTrigger($"HeavyLightAttack_{lightIndex + 1}");
            }

            lightIndex++;

            if (lightIndex > 2)
            {
                lightIndex = 0;
            }
        }

        public void RegisterHeavyAttack(Animator animator)
        {
            actionState.TrySetState(PlayerActionState.ActionState.Attacking);

            if (currentCombo == ComboType.Light)
            {
                if (!switchWeapon.equippedWeapon.isHeavy)
                {
                    animator.SetTrigger($"LightToHeavy_{lightIndex + 1}");
                }
                else
                {
                    animator.SetTrigger($"HeavyLightToHeavy_{lightIndex + 1}");
                }

                lightIndex = 0;

                currentCombo = ComboType.Heavy;
                heavyIndex = 0;
                return;
            }

            if (currentCombo != ComboType.Heavy)
            {
                heavyIndex = 0;
                currentCombo = ComboType.Heavy;
            }

            if (!switchWeapon.equippedWeapon.isHeavy)
            {
                animator.SetTrigger($"HeavyAttack_{heavyIndex + 1}");
            }
            else
            {
                animator.SetTrigger($"HeavyHeavyAttack_{heavyIndex + 1}");
            }

            heavyIndex++;

            if (heavyIndex > 2)
            {
                heavyIndex = 0;
            }
        }

        #endregion

        #region Reset States

        private void EndAttackState()
        {
            queuedAttack = QueuedAttackType.None;
            isAttacking = false;
            currentCombo = ComboType.None;
            lightIndex = 0;
            heavyIndex = 0;
            currentAttackIndex = 0;
            playerMovement.OnAttackEnd();

            if (actionState != null)
            {
                actionState.TrySetState(PlayerActionState.ActionState.Idle);
            }

        }

        public void ResetAttackState()
        {
            GetWeaponFX()?.StopAttackVFX();
            if (currentAttackCoroutine != null)
            {
                StopCoroutine(currentAttackCoroutine);
                currentAttackCoroutine = null;
            }

            StopAllCoroutines();
            isAttacking = false;
            queueNextAttack = false;
            queuedAttack = QueuedAttackType.None;
            attackFinished = true;
            canCombo = false;
            currentCombo = ComboType.None;
            currentAttackIndex = 0;
            lightIndex = 0;
            heavyIndex = 0;
            swordTip?.ResetHitFlag();
            animator.SetBool("isAttacking", false);

            if (actionState != null && actionState.CurrentState == PlayerActionState.ActionState.Attacking)
            {
                actionState.TrySetState(PlayerActionState.ActionState.Idle);
            }
        }

        #endregion

        #region Damage Multiplier for Potions

        public float GetDamageMultiplier()
        {
            return damageMultiplier;
        }

        public void SetDamageMultiplier(float multiplier)
        {
            damageMultiplier = multiplier;
        }

        #endregion
    }
}
