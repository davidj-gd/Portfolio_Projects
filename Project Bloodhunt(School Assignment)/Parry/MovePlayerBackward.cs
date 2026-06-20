using SoulsLikeProject.Combat;
using UnityEngine;

public class MovePlayerBackward : MonoBehaviour
{
    [SerializeField] PlayerCombatParry parry;

    public void OnTriggerEnter(Collider other)
    {
        if (other)
        {
            Debug.LogWarning("Calling on move back, from collider");
            parry.MoveBackward();
        }
        else
        {
            Debug.LogWarning("Triggered but not by boss");
        }
    }
}
