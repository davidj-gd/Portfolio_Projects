using SoulsLikeProject.Combat;
using SoulsLikeProject.Player;
using SoulsLikeProject.Player.Temp;
using UnityEngine;

public class PlayerHitReact : MonoBehaviour
{
    [Header("Player References")]
    [SerializeField] PlayerMovement playerMovement;
    [SerializeField] PlayerCombatParry parry;
    [SerializeField] PlayerCombatAttackHandler attackHandler;
    [SerializeField] PlayerActionState actionState;
    [SerializeField] PlayerCombatSwitchWeapon switchWeapon;
    [SerializeField] PlayerCombatLockOn lockOn;

    private Animator animator;
    private int hitIndex = 0;
    private bool knockedDown = false;
    private float hitLockout = 0.5f;
    private float lastHitTime = -10f;

    private void Awake()
    {
        animator = GetComponent<Animator>();

        if(actionState != null)
        {
            actionState.OnStateChanged += HandleStateChanged;
        }
    }

    public void PlayHitReaction()
    {
        WeaponData weapon = switchWeapon.equippedWeapon;

        if (Time.time - lastHitTime < hitLockout)
        {
            return;
        }

        lastHitTime = Time.time;

        if (knockedDown)
        {
            return;
        }

        knockedDown = true;

        if (actionState != null)
        {
            bool setState = actionState.TrySetState(PlayerActionState.ActionState.HitStun);

            if(!setState)
            {
                knockedDown = false;
                return;
            }
        }

        if (attackHandler != null)
        {
            attackHandler.ResetAttackState();
        }

        if (parry != null)
        {
            parry.ForceResetParry();
            parry.canParry = false;
        }

        animator.SetTrigger($"isHit_{hitIndex + 1}");
        hitIndex++;

        if (hitIndex > 2)
        {
            hitIndex = 0;
        }
    }

    public void EnableMovement()
    {
        if (parry != null && parry.parryActive)
        {
           parry.ForceResetParry();
        }

        if (actionState != null)
        {
            actionState.OnActionComplete();
        }
        else
        {
            knockedDown = false;
            if (parry != null)
            {
                parry.canParry = true;
            }
        }

        if (parry != null)
        {
            parry.canParry = true;
        }
    }

    private void HandleStateChanged(PlayerActionState.ActionState oldState, PlayerActionState.ActionState newState)
    {
        if (oldState == PlayerActionState.ActionState.HitStun && newState != PlayerActionState.ActionState.HitStun)
        {
            knockedDown = false;
        }
    }

    private void OnDestroy()
    {
        if (actionState != null)
        {
            actionState.OnStateChanged -= HandleStateChanged;
        }
    }
}
