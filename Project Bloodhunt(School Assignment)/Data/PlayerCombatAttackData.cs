using UnityEngine;

[CreateAssetMenu(fileName = "Player Combat Attack Data", menuName = "Player Combat/AttackData")]
public class PlayerCombatAttackData : ScriptableObject
{
    [Header("Attack Name")]
    [SerializeField] public string attackName;

    [Header("Damage / Knockback")]
    [SerializeField] public float damage;
    [SerializeField] public float knockbackForce;

    [Header("Stamina Cost")]
    [SerializeField] public float staminaCost;

    [Header("Range / Angle")]
    [SerializeField] public float attackRange;
    [SerializeField] public float attackAngle;
    [SerializeField] public bool verticalArc;

    [Header("Hitbox Debug")]
    [SerializeField] public float hitboxRadius;
    [SerializeField] public Color hitboxColor;
    public bool useSphereCast;

    [Header("Visualizer Curve")]
    public AnimationCurve arcCurve;
}
