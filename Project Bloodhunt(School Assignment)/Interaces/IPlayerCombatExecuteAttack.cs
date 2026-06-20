using System.Collections;
using UnityEngine;

namespace SoulsLikeProject.Combat.Interfaces
{
    public interface IPlayerCombatExecuteAttack
    {
        IEnumerator ExecuteAttack(IPlayerCombatHandler handler, PlayerCombatAttackData attack, int comboIndex);
    }
}


