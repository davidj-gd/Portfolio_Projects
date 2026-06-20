using System.Collections;
using UnityEngine;
using SoulsLikeProject.Combat;
using SoulsLikeProject.Player;

public class AnimationLayerHandler : MonoBehaviour
{
    [Tooltip("Current Animation Layer State")]
    public enum AnimLayerStates
    {
        LightWeapon,
        HeavyWeapon,
        Unarmed,
        SwitchWeapon,
        Attacking,
        LockOn,
        UsingPotion
    }

    [SerializeField] private AnimLayerStates currentLayer = AnimLayerStates.Unarmed;

    [Header("References")]
    [SerializeField] Animator animator;
    [SerializeField] PlayerCombatAttackHandler attackHandler;
    [SerializeField] PlayerCombatSwitchWeapon switchWeapon;
    [SerializeField] PlayerCombatLockOn lockOn;
    [SerializeField] PlayerActionState actionState;
    [SerializeField] PlayerHitReact hitReact;

    [Header("Blend Settings")]
    [SerializeField] float layerBlendSpeed = 8f;

    private WeaponData CurrentWeapon => switchWeapon != null ? switchWeapon.equippedWeapon : null;
    
    float targetUnarmed = 0f;
    float targetLight = 1f;
    float targetHeavy = 0f;
    float targetUpper = 0f;
    float targetLower = 0f;
    
    int idxLight = 0;
    int idxHeavy = -1;
    int idxUnarmed = -1;
    int idxUpper = -1;
    int idxLower = -1;

    private void Awake()
    {
        if (animator == null) animator = GetComponent<Animator>();

        if (switchWeapon == null) switchWeapon = GetComponent<PlayerCombatSwitchWeapon>();

        idxUnarmed = animator.GetLayerIndex("Unarmed Layer");
        idxLight = animator.GetLayerIndex("Base Layer");
        idxHeavy = animator.GetLayerIndex("Heavy Layer");
        idxUpper = animator.GetLayerIndex("Upperbody");
        idxLower = animator.GetLayerIndex("Lowerbody");

        actionState.OnStateChanged += HandleActionStateChanged;

    }

    private void Update()
    {
        animator.SetBool("isLockedOn", lockOn.isLockedOn);

        UpdateLayerState();
        ApplyLayer();

        LerpLayer(idxUpper, ref targetUpper);
        LerpLayer(idxLower, ref targetLower);
        LerpLayer(idxUnarmed, ref targetUnarmed);
        LerpLayer(idxLight, ref targetLight);
        LerpLayer(idxHeavy, ref targetHeavy);
    }

    public void ApplyLayer()
    {
        switch (currentLayer)
        {
            case AnimLayerStates.Unarmed:
                SetTargetWeights(unarmedWeight: 1f, lightWeight: 0f, heavyWeight: 0f, upperWeight: 0f, lowerWeight: 0f);
                break;

            case AnimLayerStates.LightWeapon:
                SetTargetWeights(unarmedWeight: 0f, lightWeight: 1f, heavyWeight: 0f, upperWeight: 0f, lowerWeight: 0f);
                break;

            case AnimLayerStates.HeavyWeapon:
                SetTargetWeights(unarmedWeight: 0f, lightWeight: 0f, heavyWeight: 1f, upperWeight: 0f, lowerWeight: 0f);
                break;

            case AnimLayerStates.SwitchWeapon:
                SetTargetWeights(unarmedWeight: 0f, lightWeight: 0f, heavyWeight: 0f, upperWeight: 1f, lowerWeight: 1f);
                break;

            case AnimLayerStates.UsingPotion:
                // Upper body only: potion animation plays, lower body continues with weapon movement
                // Keep weapon layers active for lower body movement
                if (switchWeapon.isUnarmed)
                {
                    SetTargetWeights(unarmedWeight: 1f, lightWeight: 0f, heavyWeight: 0f, upperWeight: 1f, lowerWeight: 0f);
                }
                else if (CurrentWeapon != null && CurrentWeapon.isHeavy)
                {
                    SetTargetWeights(unarmedWeight: 0f, lightWeight: 0f, heavyWeight: 1f, upperWeight: 1f, lowerWeight: 0f);
                }
                else
                {
                    SetTargetWeights(unarmedWeight: 0f, lightWeight: 1f, heavyWeight: 0f, upperWeight: 1f, lowerWeight: 0f);
                }
                break;

            case AnimLayerStates.Attacking:

                if (CurrentWeapon.isHeavy)
                {
                    SetTargetWeights(unarmedWeight: 0f, lightWeight: 0f, heavyWeight: 1f, upperWeight: 0f, lowerWeight: 0f);
                }
                else
                {
                    SetTargetWeights(unarmedWeight: 0f, lightWeight: 1f, heavyWeight: 0f, upperWeight: 0f, lowerWeight: 0f);
                }
                break;
        }
    }

    private void SetTargetWeights(float unarmedWeight, float lightWeight, float heavyWeight, float upperWeight, float lowerWeight)
    {
        targetUnarmed = unarmedWeight;
        targetLight = lightWeight;
        targetHeavy = heavyWeight;
        targetUpper = upperWeight;
        targetLower = lowerWeight;
    }

    private void LerpLayer(int layerIndex, ref float target)
    {
        if (layerIndex == -1) return;
        float current = animator.GetLayerWeight(layerIndex);
        float next = Mathf.Lerp(current, target, Time.deltaTime * layerBlendSpeed);
        animator.SetLayerWeight(layerIndex, next);
    }

    public void UpdateLayerState()
    {

        if (currentLayer == AnimLayerStates.SwitchWeapon || currentLayer == AnimLayerStates.UsingPotion) return;

        if (!actionState.IsIdle)
        {

            if (actionState.IsHitStun)
            {
                currentLayer = CurrentWeapon == null ? AnimLayerStates.Unarmed : CurrentWeapon.isHeavy ? AnimLayerStates.HeavyWeapon : AnimLayerStates.LightWeapon;
                return;
            }

            if (actionState.IsAttacking)
            {
                currentLayer = AnimLayerStates.Attacking;
                return;
            }

        }

        if (actionState.IsUsingPotion)
        {
            currentLayer = AnimLayerStates.UsingPotion;
            return;
        }

        if (switchWeapon.isUnarmed)
        {
            currentLayer = AnimLayerStates.Unarmed;
        }
        else if (CurrentWeapon.isHeavy)
        {
            currentLayer = AnimLayerStates.HeavyWeapon;
        }
        else
        {
            currentLayer = AnimLayerStates.LightWeapon;
        }

    }

    public void RequestSwitching()
    {
        currentLayer = AnimLayerStates.SwitchWeapon;
    }

    public void RevertAfterSwitch(bool nowUnarmed)
    {
        if(nowUnarmed)
        {
            currentLayer = AnimLayerStates.Unarmed;
        }
        else if (CurrentWeapon.isHeavy)
        {
            currentLayer = AnimLayerStates.HeavyWeapon;
        }
        else
        {
            currentLayer = AnimLayerStates.LightWeapon;
        }
    }

    public void RequestAttackLayers(bool start)
    {
        if (start)
        {
            currentLayer = AnimLayerStates.Attacking;
        }
        else
        {
            currentLayer = CurrentWeapon.isHeavy ? AnimLayerStates.HeavyWeapon : AnimLayerStates.LightWeapon;
        }
    }

    public void RevertAllLayers()
    {
        if (CurrentWeapon == null)
        {
            currentLayer = AnimLayerStates.Unarmed;
        }
        else if (CurrentWeapon.isHeavy)
        {
            currentLayer = AnimLayerStates.HeavyWeapon;
        }
        else
        {
            currentLayer = AnimLayerStates.LightWeapon;
        }
    }

    private void HandleActionStateChanged(PlayerActionState.ActionState oldstate, PlayerActionState.ActionState newState)
    {
        switch (newState)
        {
            case PlayerActionState.ActionState.HitStun:
                hitReact.PlayHitReaction();
                break;
        }
    }
}