using UnityEngine;
using System.Collections;

public class BloodDecalSpawner : MonoBehaviour
{
    [Header("Enemy Hit Decals")]
    [SerializeField] private GameObject[] enemyHitDecals;

    [Header("Ground Puddle Decals")]
    [SerializeField] private GameObject[] groundPuddleDecals;

    [SerializeField] private float spawnDelay = 0.05f;
    [SerializeField] private float outwardOffset = 0.03f;
    [SerializeField] private float randomRotation = 360f;

    public void SpawnEnemyDecal(Vector3 pos, Vector3 normal)
    {
        StartCoroutine(SpawnRoutine(enemyHitDecals, pos, normal));
    }

    public void SpawnGroundDecal(Vector3 pos, Vector3 normal)
    {
        StartCoroutine(SpawnRoutine(groundPuddleDecals, pos, normal));
    }

    private IEnumerator SpawnRoutine(GameObject[] decalArray, Vector3 pos, Vector3 normal)
    {
        yield return new WaitForSeconds(spawnDelay);

        if (decalArray == null || decalArray.Length == 0)
            yield break;

        GameObject prefab = decalArray[Random.Range(0, decalArray.Length)];

        Vector3 spawnPos = pos + normal * outwardOffset;

        Quaternion rot = Quaternion.LookRotation(-normal);
        rot *= Quaternion.Euler(0f, 0f, Random.Range(0f, randomRotation));

        GameObject decal = Instantiate(prefab, spawnPos, rot);

        var fade = decal.GetComponent<DecalFade>();
        if (fade != null)
            fade.BeginFade(8f);
    }
}