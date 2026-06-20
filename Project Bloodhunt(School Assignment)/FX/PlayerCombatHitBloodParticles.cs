using UnityEngine;
using UnityEngine.VFX;

namespace SoulsLikeProject.Combat
{
    public class PlayerCombatHitBloodParticles : MonoBehaviour
    {
        [SerializeField] private VisualEffect bloodPrefab; 

        public void SpawnBlood(Vector3 position, Quaternion rotation)
        {
            VisualEffect vfx = Instantiate(bloodPrefab, position, rotation);
            vfx.Play();
            Destroy(vfx.gameObject, 2f); 
        }

    }
}