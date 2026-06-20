using UnityEngine;
using SoulsLikeProject.Combat;

public class PlayerStashWeapon : MonoBehaviour
{
    [SerializeField] PlayerCombatSwitchWeapon switchWeapon;
    [SerializeField] Transform heavyStashSlot;
    [SerializeField] Transform lightStashSlot;

    private GameObject stashedInstance;
    public bool isStashed { get; private set; } = false;

    void Start()
    {
        if (switchWeapon == null) switchWeapon = GetComponent<PlayerCombatSwitchWeapon>();

        if (switchWeapon.startUnarmed)
        {
            StashWeaponPrefab();
        }    
    }

    public void StashWeaponPrefab()
    {
        if (isStashed) return;

        if (switchWeapon == null) return;

        WeaponData data = switchWeapon.equippedWeapon != null
            ? switchWeapon.equippedWeapon
            : switchWeapon.startingWeapon;

        if (data == null || data.prefab == null) return;

        Transform slot = data.isHeavy && heavyStashSlot != null
            ? heavyStashSlot
            : lightStashSlot;

        if (slot == null) return;

        stashedInstance = Instantiate(data.prefab, slot.position, slot.rotation, slot);
        isStashed = true;
    }

    public void UnstashWeaponPrefab()
    {
        if (!isStashed) return;

        if (stashedInstance != null)
        {
            Destroy(stashedInstance);
            stashedInstance = null;
        }

        isStashed = false;
    }
}