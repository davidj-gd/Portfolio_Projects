using UnityEngine;
using SoulsLikeProject.Combat;

public class VfxManager : MonoBehaviour
{
    public static VfxManager Instance { get; private set; }

    [SerializeField] private BloodVFXSpawner bloodVfxSpawner;

    private void Awake()
    {
        if (Instance != null && Instance != this)
        {
            Destroy(gameObject);
            return;
        }

        Instance = this;

        if (bloodVfxSpawner == null)
            bloodVfxSpawner = Object.FindFirstObjectByType<BloodVFXSpawner>();
    }

    public void PlayHitVfx(PlayerCombatHitData data)
    {
        if (bloodVfxSpawner == null)
        {
            return;
        }

        bloodVfxSpawner.SpawnEnemyHitVfx(data.position, data.normal, data.splashDir);
    }
}