using UnityEngine;
using System.Collections.Generic;


[System.Serializable]
public class DamageCalculationResult
{
    public float baseDamage;
    public float hitboxMultiplier;
    public float brokenLimbMultiplier;
    public bool isCritical;
    public float criticalMultiplier;
    public float finalDamage;
    
    public float GetTotalMultiplier()
    {
        float total = hitboxMultiplier;
        if (brokenLimbMultiplier > 1f)
        {
            total *= brokenLimbMultiplier;
        }
        if (isCritical)
        {
            total *= criticalMultiplier;
        }
        return total;
    }
}

[CreateAssetMenu(fileName = "Hitbox Profile", menuName = "Combat/Hitbox Profile")]
public class CombatHitboxProfile : ScriptableObject
{
    [System.Serializable]
    public class HitboxMultiplier
    {
        public string hitboxName;
        public float damageMultiplier;
        
        [Header("Broken Limb multipliers")]
        [Tooltip("Damage multiplier when this hitbox's limb is broken (added to the base multiplier)")]
        public float brokenLimbDamageMultiplier = 1.5f;
        
        [Header("Critical Hit Settings")]
        [Tooltip("Base critical hit chance for this hitbox (0.1 = 10% chance")]
        [Range(0f, 1f)]
        public float baseCriticalChance = 0.5f;
        
        [Tooltip("Critical Hit chance when limb ius below critical threshold (0.1 = 10% chance)")]
        [Range(0f, 1f)]
        public float lowHealthCriticalChance = 0.3f;
    
        [Tooltip("Critical hit chance when limb is broken (0-1)")]
        [Range(0f, 1f)]
        public float brokenLimbCriticalChance = 0.5f;
    
        [Tooltip("Damage multiplier when critical hit occurs ( 2.0 = double damage)")]
        public float criticalDamageMultiplier = 2f;
    
        
    }

    public List<HitboxMultiplier> multipliers = new List<HitboxMultiplier>();

    public float GetMultiplier(string hitboxName)
    {
        foreach (var hitboxMultiplier in multipliers)
        {
            if (hitboxMultiplier.hitboxName == hitboxName)
            {
                return hitboxMultiplier.damageMultiplier;
            }
        }
        return 1f; 
    }

    public float GetBrokenLimbMultiplier(string hitboxName)
    {
        foreach (var hitboxMultiplier in multipliers)
        {
            if (hitboxMultiplier.hitboxName == hitboxName)
            {
                return hitboxMultiplier.brokenLimbDamageMultiplier;
            }
        }
        return 1f;
    }

    public float GetCriticalChance(string hitboxName, bool isLimbBroken, bool isLimbLowHealth)
    {
        foreach (var hitboxMultiplier in multipliers)
        {
            if (hitboxMultiplier.hitboxName == hitboxName)
            {
                if (isLimbBroken)
                {
                    return hitboxMultiplier.brokenLimbCriticalChance;
                }
                else if (isLimbLowHealth)
                {
                    return hitboxMultiplier.lowHealthCriticalChance;
                }
                else
                {
                    return hitboxMultiplier.baseCriticalChance;
                }
            }
        }
        return 0f;
    }

    public float GetCriticalDamageMultiplier(string hitboxName)
    {
        foreach (var hitboxMultiplier in multipliers)
        {
            if (hitboxMultiplier.hitboxName == hitboxName)
            {
                return hitboxMultiplier.criticalDamageMultiplier;
            }
        }

        return 2f;
    }
    
    public DamageCalculationResult CalculateDamage(
        string hitboxName, 
        float baseDamage, 
        bool isLimbBroken, 
        bool isLimbLowHealth)
    {
        DamageCalculationResult result = new DamageCalculationResult
        {
            baseDamage = baseDamage,
            hitboxMultiplier = GetMultiplier(hitboxName),
            brokenLimbMultiplier = 1f,
            isCritical = false,
            criticalMultiplier = 1f,
            finalDamage = baseDamage
        };
    
        float hitboxMult = result.hitboxMultiplier;
        float damageAfterHitbox = baseDamage * hitboxMult;
    
        if (isLimbBroken)
        {
            result.brokenLimbMultiplier = GetBrokenLimbMultiplier(hitboxName);
            damageAfterHitbox *= result.brokenLimbMultiplier;
        }
    
        float critChance = GetCriticalChance(hitboxName, isLimbBroken, isLimbLowHealth);
        if (critChance > 0f && Random.value <= critChance)
        {
            result.isCritical = true;
            result.criticalMultiplier = GetCriticalDamageMultiplier(hitboxName);
            damageAfterHitbox *= result.criticalMultiplier;
        }
    
        result.finalDamage = damageAfterHitbox;
        return result;
    }   
    
}
