using UnityEngine;
using System;

namespace SoulsLikeProject.Player
{
    public class PlayerActionState : MonoBehaviour
    {
        public enum ActionState
        {
            Idle,
            Attacking,
            Parrying,
            Dashing,
            UsingPotion,
            HitStun
        }

        [Header("Current State")]
        [SerializeField] private ActionState currentState = ActionState.Idle;

        [Header("Safety")]
        [SerializeField] private float stateTimeout = 5f;
        private float stateTimer = 0f;

        [Header("References")]
        [SerializeField] Animator animator;

        public ActionState CurrentState => currentState;
        public bool IsIdle => currentState == ActionState.Idle;
        public bool IsAttacking => currentState == ActionState.Attacking;
        public bool IsParrying => currentState == ActionState.Parrying;
        public bool IsDashing => currentState == ActionState.Dashing;
        public bool IsHitStun => currentState == ActionState.HitStun;
        public bool IsUsingPotion => currentState == ActionState.UsingPotion;

        public bool CanDoState => currentState == ActionState.Idle;
        public bool CanBeInterrupted => currentState == ActionState.Attacking || currentState == ActionState.Parrying;

        public event Action <ActionState, ActionState> OnStateChanged;
        public event Action<bool> OnMovementAllowed;

        private void Awake()
        {
            if (animator == null)
            {
                animator = GetComponent<Animator>();
            }
        }

        private void Update()
        {
            if (currentState != ActionState.Idle)
            {
                stateTimer += Time.deltaTime;

                float timeoutReset = currentState switch
                {
                    ActionState.Attacking => 3f,
                    ActionState.Parrying => 2f,
                    ActionState.Dashing => 1.5f,
                    ActionState.HitStun => 2f,
                    ActionState.UsingPotion => 3f,
                    _ => 5f
                };

                if (stateTimer >= timeoutReset)
                {
                    ForceIdle();
                }
            }
        }

        public bool TrySetState(ActionState newState)
        {
            switch (currentState)
            {
                case ActionState.Idle:
                    {
                        return SetState(newState);
                    }
                    
                case ActionState.Attacking:

                    if (newState == ActionState.HitStun || newState == ActionState.Idle)
                    {
                        return SetState(newState);
                    }

                    return false;

                case ActionState.Parrying:

                    if (newState == ActionState.HitStun || newState == ActionState.Idle || newState == ActionState.Attacking)
                    {
                        return SetState(newState);
                    }

                    return false;

                case ActionState.Dashing:

                    if (newState == ActionState.HitStun || newState == ActionState.Idle)
                    {
                        return SetState(newState);
                    }

                    return false;

                case ActionState.HitStun:

                    if (newState == ActionState.HitStun || newState == ActionState.Idle)
                    {
                        return SetState(newState);
                    }

                    return false;

                case ActionState.UsingPotion:

                    if (newState == ActionState.Idle || newState == ActionState.HitStun)
                    {
                        return SetState(newState);
                    }

                return false;

                default:

                    {
                        return false;
                    }
            }
        }

        #region Bools

        public bool ValidateState()
        {
            return true;
        }
       
        private bool SetState(ActionState newState)
        {
            if (currentState == newState) return true;

            ActionState oldState = currentState;
            currentState = newState;
            stateTimer = 0f;

            OnStateChanged?.Invoke(oldState, newState);

            bool movementAllowed = (currentState == ActionState.Idle);
            OnMovementAllowed?.Invoke(movementAllowed);

            return true;
        }

        #endregion

        public void ForceIdle()
        {
            SetState(ActionState.Idle);

            if (animator != null)
            {
                animator.SetBool("isAttacking", false);
                animator.ResetTrigger("Parry");
                animator.ResetTrigger("Dash");
                animator.ResetTrigger("LightAttack_1");
                animator.ResetTrigger("LightAttack_2");
                animator.ResetTrigger("LightAttack_3");
                animator.ResetTrigger("HeavyAttack_1");
                animator.ResetTrigger("HeavyAttack_2");
                animator.ResetTrigger("HeavyAttack_3");
                animator.ResetTrigger("isHit_1");
                animator.ResetTrigger("isHit_2");
                animator.ResetTrigger("isHit_3");
                animator.ResetTrigger("UsePotion");
            }
        }

        public void OnActionComplete()
        {
            SetState(ActionState.Idle);
        }
    }
}

