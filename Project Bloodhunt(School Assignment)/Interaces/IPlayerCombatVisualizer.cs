using UnityEngine;


namespace SoulsLikeProject.Combat
{
    public interface IPlayerCombatVisualizer 
    {
        void ShowAttack(Transform origin, PlayerCombatAttackData attack);
    }
}
