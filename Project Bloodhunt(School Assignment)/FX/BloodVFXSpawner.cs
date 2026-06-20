using UnityEngine;
using UnityEngine.VFX;

public class BloodVFXSpawner : MonoBehaviour
{
    [SerializeField] private VisualEffect[] vfxPrefabs;   
    [SerializeField] private float lifetime = 2f;

    public void SpawnEnemyHitVfx(Vector3 pos, Vector3 normal, Vector3 splashDir)
    {
        if (vfxPrefabs == null || vfxPrefabs.Length == 0)
        {
            Debug.LogWarning("BloodVFXSpawner: No VFX prefabs assigned.");
            return;
        }

        VisualEffect prefab = vfxPrefabs[Random.Range(0, vfxPrefabs.Length)];

        Quaternion rot = Quaternion.LookRotation(splashDir, normal);
        VisualEffect vfx = Instantiate(prefab, pos + normal * 0.05f, rot);

        vfx.SendEvent("OnPlay");

        Destroy(vfx.gameObject, lifetime);
    }
}