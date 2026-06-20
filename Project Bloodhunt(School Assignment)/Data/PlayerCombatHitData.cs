using UnityEngine;

namespace SoulsLikeProject.Combat
{

    public struct PlayerCombatHitData
    {
        public Vector3 position;
        public Vector3 normal;
        public Vector3 splashDir;
        public float damage;
        public string hitboxName;
        public PlayerCombatAttackData attackData;
        public float timestamp;
    }

}
