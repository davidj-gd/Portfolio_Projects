using UnityEngine;
using UnityEngine.InputSystem;

namespace SoulsLikeProject.Player.Placeholder
{
    public class PlayerCombatEquipWeapon : MonoBehaviour
    {
        [Header("Weapon Prefab")]
        [SerializeField] GameObject weaponModelPrefab;

        [Header("Weapon Hand Socket")]
        [SerializeField] Transform weaponSlot;

        private bool isWeaponEquipped = false;
        private GameObject spawnedWeapon;

        private void Start()
        {
            if (weaponSlot == null)
            {
                return;
            }

            if (weaponSlot.childCount > 0)
            {
                spawnedWeapon = weaponSlot.GetChild(0).gameObject;
            }
            else if (weaponModelPrefab != null)
            {
                spawnedWeapon = Instantiate(weaponModelPrefab, weaponSlot);
            }

            if (spawnedWeapon != null)
            {
                spawnedWeapon.SetActive(false);
            }
            else
            {

            }
        }
        public void OnEquip(InputAction.CallbackContext context)
        {
            if (context.performed)
            {
                EquipWeapon();
            }
        }

        private void EquipWeapon()
        {
            if(!isWeaponEquipped && weaponModelPrefab != null)
            {
                weaponModelPrefab.SetActive(true);
                isWeaponEquipped = true;
            }
        }
    }

}
