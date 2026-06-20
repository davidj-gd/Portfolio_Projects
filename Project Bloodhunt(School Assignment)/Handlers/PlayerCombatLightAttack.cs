using SoulsLikeProject.Combat.Interfaces;
using System.Collections;
using UnityEngine;

namespace SoulsLikeProject.Combat
{
    public class PlayerCombatLightAttack : MonoBehaviour, IPlayerCombatHandler
    {
        [SerializeField] PlayerCombatAttackHandler attackHandler;
        [SerializeField] PlayerCombatSwitchWeapon switchWeapon;

        float attackTimeout = 5f;
        float timeElapsed = 0f;

        private Coroutine currentCoroutine = null;

        public IEnumerator PerformAttack(PlayerCombatAttackData attack, PlayerCombatSwordTip swordTip, Animator animator)
        {
            timeElapsed = 0f;
            attackHandler.attackFinished = false;

            if (swordTip != null)
            {
                swordTip.SetAttack(attack);
            }

            attackHandler.RegisterLightAttack(animator);

            animator.SetBool("isAttacking", true);

            while (!attackHandler.attackFinished && timeElapsed < attackTimeout)
            {
                timeElapsed += Time.deltaTime;
                yield return null;
            }

            if (!attackHandler.attackFinished)
            {
                attackHandler.AttackEnded();
            }

            currentCoroutine = null;
        }

        public void CancelAttack()
        {
            if (currentCoroutine != null)
            {
                StopCoroutine(currentCoroutine);
                currentCoroutine = null;
            }

            StopAllCoroutines();
        }
    }
}

