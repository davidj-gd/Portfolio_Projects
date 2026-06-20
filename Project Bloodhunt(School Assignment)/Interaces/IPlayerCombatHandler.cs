using System.Collections;
using UnityEngine;

namespace SoulsLikeProject.Combat.Interfaces
{
    public interface IPlayerCombatHandler
    {
        IEnumerator PerformAttack(PlayerCombatAttackData attack, PlayerCombatSwordTip swordTip, Animator animator);
    }
}


