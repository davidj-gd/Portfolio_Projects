using SoulsLikeProject.Boss.Health;
using SoulsLikeProject.Player.Temp;
using SoulsLikeProject.Quests;
using System.Collections.Generic;
using Unity.Cinemachine;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerCombatLockOn : MonoBehaviour
{
    [Header("References")]
    [SerializeField] private PlayerController_Copy playerController;
    [SerializeField] private Animator animator;
    [SerializeField] private PlayerHealth playerHealth;

    [Header("UI Dot")]
    [SerializeField] private GameObject lockOnDotUI;

    [Header("Settings")]
    [SerializeField][Range(5, 50)] float enemyDetectionRange = 50f;
    [SerializeField] private LayerMask bossLayer;

    [Header("Camera")]
    [SerializeField] private CinemachineCamera freeCam;
    [SerializeField] private CinemachineCamera bossCam;
    [SerializeField] private CinemachineInputAxisController freeLookInput;
    [SerializeField] private CinemachineTargetGroup targetGroup;

    public Transform currentTarget;
    public bool isLockedOn = false;

    private BossHealth bossHealth;

    private void Awake()
    {
        if (playerHealth == null)
        {
            playerHealth = GetComponent<PlayerHealth>();
        }

        playerHealth.OnDeath += () =>
        {
            if (isLockedOn)
            {
                isLockedOn = false;
                if (currentTarget != null)
                {
                    targetGroup.RemoveMember(currentTarget);
                }
                currentTarget = null;
                SyncOrbitalFollow(freeCam, bossCam);
                freeCam.Priority = 10;
                bossCam.Priority = 5;
                lockOnDotUI.SetActive(false);
                freeLookInput.enabled = true;
                animator.SetBool("isLockedOn", false);
            }
        };
    }

    private void Start()
    {
        lockOnDotUI.SetActive(false);
    }

    private void LateUpdate()
    {
        if (isLockedOn && currentTarget != null)
        {
            UpdateDynamicLockPoint();
        }
    }

    public void LockCamera(InputAction.CallbackContext context)
    {
        if (!context.performed)
        {
            return;
        }

        if (QuestManager.Instance.CurrentState == QuestState.InHub)
        {
            return;
        }

        if (!isLockedOn)
        {
            Collider[] hits = Physics.OverlapSphere(transform.position, enemyDetectionRange, bossLayer);
            float closestDistance = Mathf.Infinity;
            Transform closest = null;

            foreach (var hit in hits)
            {
                float dist = Vector3.Distance(transform.position, hit.transform.position);
                if (dist < closestDistance)
                {
                    closestDistance = dist;
                    closest = hit.transform;
                }
            }

            if (closest == null)
            {
                return;
            }

            Transform lockPoint = GetLockPoint(closest);

            if (lockPoint != null)
            {
                currentTarget = lockPoint;
            }
            else
            {
                currentTarget = closest;
            }

            isLockedOn = true;
            targetGroup.AddMember(currentTarget, 1f, 2f);

            freeCam.Priority = 5;
            bossCam.Priority = 10;

            lockOnDotUI.SetActive(true);
            freeLookInput.enabled = false;
            animator.SetBool("isLockedOn", true);

            bossHealth = currentTarget.root.GetComponent<BossHealth>();
            if (bossHealth != null)
            {
                bossHealth.OnDeath += () =>
                {
                    if (isLockedOn)
                    {
                        isLockedOn = false;
                        if (currentTarget != null)
                        {
                            targetGroup.RemoveMember(currentTarget);
                        }
                        currentTarget = null;
                        SyncOrbitalFollow(freeCam, bossCam);
                        freeCam.Priority = 10;
                        bossCam.Priority = 5;
                        lockOnDotUI.SetActive(false);
                        freeLookInput.enabled = true;
                        animator.SetBool("isLockedOn", false);
                    }
                };
            }
        }
        else
        {
            if(bossHealth != null)
            {
                bossHealth.OnDeath = null;
                bossHealth = null;
            }

            isLockedOn = false;

            if (currentTarget != null)
            {
                targetGroup.RemoveMember(currentTarget);
            }

            currentTarget = null;

            SyncOrbitalFollow(freeCam, bossCam);

            freeCam.Priority = 10;
            bossCam.Priority = 5;

            lockOnDotUI.SetActive(false);
            freeLookInput.enabled = true;
            animator.SetBool("isLockedOn", false);
        }
    }

    private void UpdateDynamicLockPoint()
    {
        Transform enemyRoot = currentTarget.root;

        Transform lockPoint = GetLockPoint(enemyRoot);

        if (lockPoint == null)
        {
            return;
        }

        if (lockPoint != currentTarget)
        {
            targetGroup.RemoveMember(currentTarget);
            currentTarget = lockPoint;
            targetGroup.AddMember(currentTarget, 1f, 2f);
        }
    }

    #region Helpers

    private Transform GetLockPoint(Transform root)
    {
        if (root == null)
        {
            return null;
        }

        foreach (Transform child in root.GetComponentsInChildren<Transform>())
        {
            if (child.name.StartsWith("LockOnTarget"))
            {
                return child;
            }
        }

        return null;
    }

    private void SyncOrbitalFollow(CinemachineCamera free, CinemachineCamera boss)
    {
        var freeOrbital = free.GetComponent<CinemachineOrbitalFollow>();

        freeCam.transform.rotation = bossCam.transform.rotation;

        Vector3 euler = freeCam.transform.eulerAngles;

        freeOrbital.HorizontalAxis.Value = euler.y;
        freeOrbital.VerticalAxis.Value = euler.x;
    }

    #endregion
}