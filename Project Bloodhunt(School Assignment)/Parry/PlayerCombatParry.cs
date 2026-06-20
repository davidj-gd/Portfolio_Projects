using SoulsLikeProject.Player;
using SoulsLikeProject.Player.Temp;
using System.Collections;
using System.Net.Mail;
using UnityEngine;
using UnityEngine.InputSystem;

namespace SoulsLikeProject.Combat
{
    public class PlayerCombatParry : MonoBehaviour
    {
        [Header("Parry Box Prefab")]
        [SerializeField] GameObject parryBox;

        [Header("Parry Settings")]
        [SerializeField] float parryTimeout = 1.5f;
        [SerializeField] float parryCooldown = 1.5f;

        [Header("Player References")]
        [SerializeField] PlayerMovement playerMovement;
        [SerializeField] PlayerCombatAttackHandler attackHandler;
        [SerializeField] PlayerActionState actionState;
        [SerializeField] PlayerCombatSwitchWeapon switchWeapon;

        [Header("Parry Knockback Settings")]
        [SerializeField] float parryKnockbackDistance = 1.2f;
        [SerializeField] float parryKnockbackDuration = 0.25f;

        public bool parryActive = false;
        public bool canParry = true;

        private Animator animator;
        private Coroutine parryTimeoutCoroutine = null;
        private Coroutine moveBackwardCoroutine = null;
        private float lastParryTime = -999f;

        private void Start()
        {
            animator = GetComponent<Animator>();
            parryBox.SetActive(false);

            if (attackHandler == null)
            {
                Debug.LogWarning("Couldnt find attack handler");
                attackHandler = GetComponent<PlayerCombatAttackHandler>();
            }
        }

        public void OnParry(InputAction.CallbackContext context)
        {
            if(context.performed)
            {
                if (switchWeapon.isUnarmed) return;

                if (actionState != null)
                {
                    if (!actionState.CanDoState) return;

                    if (!actionState.TrySetState(PlayerActionState.ActionState.Parrying))
                    {
                        return;
                    }
                }
                else
                {
                    if (actionState != null & actionState.IsParrying)
                    {
                        return;
                    }
                }

                if (attackHandler != null && attackHandler.isAttacking)
                {
                    attackHandler.ResetAttackState();
                }

                if (Time.time < lastParryTime + parryCooldown)
                {
                    return;
                }

                canParry = false;

                if (!switchWeapon.equippedWeapon.isHeavy)
                {
                    animator.SetTrigger("Parry");
                }
                else
                {
                    animator.SetTrigger("ParryHeavy");
                }

                lastParryTime = Time.time;

                if(parryTimeoutCoroutine != null)
                {
                    StopCoroutine(parryTimeoutCoroutine);
                }

                parryTimeoutCoroutine = StartCoroutine(ParryTimeoutSafety());
            } 
        }

        private IEnumerator ParryTimeoutSafety()
        {
            yield return new WaitForSeconds(parryTimeout);

            if(parryActive)
            {
                ForceResetParry();
            }
            else
            {
                canParry = true;
            }

            parryTimeoutCoroutine = null;
        }

        public void ParryEnabled()
        {
            parryBox.SetActive(true);
            parryActive = true;

        }


        public void ParryDisabled()
        {
            parryBox.SetActive(false);

            if (parryTimeoutCoroutine != null)
            {
                StopCoroutine(parryTimeoutCoroutine);
                parryTimeoutCoroutine = null;
            }

            parryActive = false;
            canParry = true;

            if (actionState != null && actionState.CurrentState == PlayerActionState.ActionState.Parrying)
            {
                actionState.TrySetState(PlayerActionState.ActionState.Idle);
            }
            
        }

        public void ForceResetParry()
        {
            if (parryTimeoutCoroutine != null)
            {
                StopCoroutine(parryTimeoutCoroutine);
                parryTimeoutCoroutine = null;
            }

            if (moveBackwardCoroutine != null)
            {
                StopCoroutine(moveBackwardCoroutine);
                moveBackwardCoroutine = null;
            }

            parryBox.SetActive(false);
            parryActive = false;
            canParry = true;

            if (actionState != null && actionState.CurrentState == PlayerActionState.ActionState.Parrying)
            {
                actionState.TrySetState(PlayerActionState.ActionState.Idle);
            }
        }

        public void MoveBackward()
        {
            moveBackwardCoroutine = StartCoroutine(MoveBackwardRoutine());
        }

        private IEnumerator MoveBackwardRoutine()
        {
            if (playerMovement == null) yield break;

            float elapsedTime = 0f;
            Vector3 backwardDirection = -transform.forward;

            while (elapsedTime < parryKnockbackDuration)
            {
                float progress = elapsedTime / parryKnockbackDuration;

                float easeProgress = 1f - Mathf.Pow(1f - progress, 2f);

                float velocityMagnitude = (parryKnockbackDistance / parryKnockbackDuration) * easeProgress;
                Vector3 velocity = backwardDirection * velocityMagnitude;

                playerMovement.AddVelocity(velocity);

                elapsedTime += Time.deltaTime;
                yield return null;
            }

            playerMovement.ClearExternalVelocity();
            moveBackwardCoroutine = null;
        }
    }
}

