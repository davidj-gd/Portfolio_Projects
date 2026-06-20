using SoulsLikeProject.Player.Temp;
using SoulsLikeProject.Boss.Health;
using UnityEngine;
using UnityEngine.VFX;
using static UnityEngine.Analytics.IAnalytic;
using SoulsLikeProject.Inventory;
using Unity.Cinemachine;

namespace SoulsLikeProject.Combat
{
    public class PlayerCombatSwordTip : MonoBehaviour
    {
        [SerializeField] private GameObject damageTextPrefab;
        [SerializeField] private Transform playerRoot;
        [SerializeField] private PlayerCombatHitBloodParticles blood;
        [SerializeField] private PlayerCombatAttackHandler attackHandler;

        private PlayerCombatAttackData currentAttackData;
        private WeaponData currentWeaponData;
        private float finalDamage = 0f;
        public float baseDamageBonus = 0f;

        private CinemachineImpulseSource impulseSource;

        public Vector3 swordTipVelocity { get; private set; }
        private Vector3 lastTipPosition;
        public bool hasDealtDamageThisAttack = false;

        private void Awake()
        {
            playerRoot = GetComponentInParent<PlayerMovement>()?.transform;
            blood = GetComponent<PlayerCombatHitBloodParticles>();
            impulseSource = GetComponent<CinemachineImpulseSource>();

            ValidateComponents();
        }

        private void Start()
        {
            lastTipPosition = transform.position;

            if (impulseSource == null)
            {
                impulseSource = GetComponent<CinemachineImpulseSource>();
            }

            CheckAndApplyBuff();
        }

        private void Update()
        {
            Vector3 current = transform.position;
            swordTipVelocity = (current - lastTipPosition) / Mathf.Max(Time.deltaTime, 1e-6f);
            lastTipPosition = current;
        }

        public void SetAttack(PlayerCombatAttackData attack)
        {
            currentAttackData = attack;
            hasDealtDamageThisAttack = false;

            float weaponBase = currentWeaponData != null ? currentWeaponData.baseDamage : 0f;
            float baseDamage = weaponBase + (attack != null ? attack.damage : 0f) + baseDamageBonus;

            float damageMultiplier = attackHandler != null ? attackHandler.GetDamageMultiplier() : 1f;
            finalDamage = baseDamage * damageMultiplier;
        }

        public void SetWeaponData(WeaponData weaponData)
        {
            currentWeaponData = weaponData;
        }

        private void OnTriggerEnter(Collider other)
        {
            if (!attackHandler.isAttacking || currentAttackData == null) return;
 
            if (other == null || other.CompareTag("Player")) return;

            if (other.TryGetComponent(out ObjectHealth health))
            {
                health.ObjectTakeDamage(finalDamage);
                if (damageTextPrefab != null)
                {
                    var txt = Instantiate(damageTextPrefab, transform.position, Quaternion.identity);
                    var damageText = txt.GetComponent<PlayerCombatDamageText>();
                    if (damageText != null)
                    {
                        damageText.SetText(finalDamage.ToString("F0"));
                    }
                    Destroy(txt, 2f);
                }
                return;
            }

            if (other.transform.root.TryGetComponent(out ObjectHealth rootHealth))
            {
                rootHealth.ObjectTakeDamage(finalDamage);
                return;
            }

            if (attackHandler == null)
            {
                Debug.LogWarning("AttackHandler is null in PlayerCombatSwordTip. Cannot process hit.", this);
                return;
            }

            if (!attackHandler.isAttacking)
            {
                return;
            }

            if (currentAttackData == null)
            {
                Debug.Log("Blocked: currentAttackData is null", this);
                return;
            }

            Vector3 hitPosition = other.ClosestPoint(transform.position);
            Vector3 hitNormal = (hitPosition - transform.position).normalized;
            Vector3 splashDir = ComputeSplashDirection(hitPosition, hitNormal);

            blood.SpawnBlood(hitPosition + hitNormal * 0.06f, Quaternion.LookRotation(splashDir, hitNormal));
                
            VfxManager.Instance?.PlayHitVfx(new PlayerCombatHitData
            {
                position = hitPosition,
                normal = hitNormal,
                splashDir = splashDir,
                damage = finalDamage,
                hitboxName = "",
                attackData = currentAttackData,
                timestamp = Time.time
            });

            if (!hasDealtDamageThisAttack)
            {
                EnemyHitbox enemyHitbox = other.GetComponent<EnemyHitbox>();
                if (enemyHitbox != null)
                {
                    hasDealtDamageThisAttack = true;

                    CinemachineShake.Instance.cameraShake(impulseSource);

                    PlayerCombatHitData data = new PlayerCombatHitData
                    {
                        position = hitPosition,
                        normal = hitNormal,
                        splashDir = splashDir,
                        damage = finalDamage,
                        hitboxName = enemyHitbox.hitboxName,
                        attackData = currentAttackData,
                        timestamp = Time.time
                    };

                    enemyHitbox.ReceiveHit(data);

                    if (damageTextPrefab != null)
                    {
                        var txt = Instantiate(damageTextPrefab, transform.position, Quaternion.identity);
                        var damageText = txt.GetComponent<PlayerCombatDamageText>();
                        if (damageText != null)
                        {
                            damageText.SetText(finalDamage.ToString());
                            damageText.SetColor(Color.white);
                        }
                        Destroy(txt, 2f);
                    }
                }
                else
                {
                    BossLimbHitbox bossLimbHitbox = other.GetComponent<BossLimbHitbox>();
                    if (bossLimbHitbox != null)
                    {
                        hasDealtDamageThisAttack = true;

                        CinemachineShake.Instance.cameraShake(impulseSource);

                        DamageCalculationResult damageResult = bossLimbHitbox.CalculateDamageWithDetails(this.finalDamage);
                        float appliedDamage = bossLimbHitbox.ApplyDamage(this.finalDamage, gameObject);

                        if (appliedDamage > 0f)
                        {
                            Debug.Log($"Dealt {appliedDamage} damage to boss limb: {bossLimbHitbox.HitboxName} " +
                                      $"(Critical: {damageResult.isCritical}, Multiplier: {damageResult.GetTotalMultiplier():F2}x)");

                            if (damageTextPrefab != null)
                            {
                                var txt = Instantiate(damageTextPrefab, transform.position, Quaternion.identity);
                                var damageText = txt.GetComponent<PlayerCombatDamageText>();
                                if (damageText != null)
                                {
                                    damageText.SetText(appliedDamage.ToString("F0"));
            
                                    if (damageResult.isCritical)
                                    {
                                        damageText.SetColor(Color.yellow);
                                    }
                                    else
                                    {
                                        damageText.SetColor(Color.white);
                                    }
                                }
                                Destroy(txt, 2f);
                            }
                        }
                    }
                }
            }
        }

        private Vector3 ComputeSplashDirection(Vector3 hitPosition, Vector3 hitNormal)
        {
            Vector3 velocityDirection = swordTipVelocity.sqrMagnitude > 1e-4f ? swordTipVelocity.normalized : transform.forward;
            Vector3 fromPlayerToHit = (hitPosition - (playerRoot != null ? playerRoot.position : transform.position)).normalized;
            float towardEnemyDot = Vector3.Dot(velocityDirection, fromPlayerToHit);

            if (towardEnemyDot > 0.6f)
                return -fromPlayerToHit;

            Vector3 slice = Vector3.ProjectOnPlane(velocityDirection, hitNormal).normalized;
            if (slice.sqrMagnitude < 1e-4f)
                slice = Vector3.Cross(hitNormal, transform.up).normalized;

            if (Vector3.Dot(slice, hitNormal) < 0f)
                slice = -slice;

            return slice;
        }

        #region damage buff
        public void CheckAndApplyBuff()
        {
            if (HubInventoryManager.Instance == null) return;

            float totalDamageBuff = 0f;

            foreach (var inventoryItem in HubInventoryManager.Instance.Items)
            {
                if (inventoryItem?.itemData == null) continue;

                if (inventoryItem.itemData.weaponDamageIncreaseAmount > 0f)
                {
                    totalDamageBuff += inventoryItem.itemData.weaponDamageIncreaseAmount * inventoryItem.quantity;
                }
            }

            baseDamageBonus = totalDamageBuff;
        }

        private void SetDamageBonus(float baseDamageBonus)
        {
            this.baseDamageBonus = baseDamageBonus;
        }

        #endregion

        #region Reset flag
        public void ResetHitFlag() => hasDealtDamageThisAttack = false;

        #endregion

        #region Validate Components

        private void ValidateComponents()
        {
            if (playerRoot == null)
            {
                var player = GameObject.FindGameObjectWithTag("Player");
                if (player != null)
                    playerRoot = player.transform;
            }

            if (attackHandler == null)
            {
                attackHandler = Object.FindFirstObjectByType<PlayerCombatAttackHandler>();
            }

        }

        #endregion
    }
}