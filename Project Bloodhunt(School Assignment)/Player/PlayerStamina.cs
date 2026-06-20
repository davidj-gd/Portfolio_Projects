using SoulsLikeProject.Player.Temp;
using UnityEngine;

public class PlayerStamina : MonoBehaviour
{
    [Header("Stamina Settings")]
    public float maxStamina = 100f;
    public float StaminaRegenRate = 15f;
    public float regenDelay = 1f;
    private float regenTimer = 0f;

    [Header("Player Movement Reference")]
    [SerializeField] private PlayerMovement playerMovement;

    [HideInInspector] public float currentStamina;

    private void Start()
    {
        currentStamina = maxStamina;
    }

    private void Update()
    {
        HandleRegen();
    }

    public bool UseStamina(float amount)
    {
        if (currentStamina >= amount)
        {
            currentStamina -= amount;
            regenTimer = regenDelay;
            return true;
        }
        return false;
    }

    private void HandleRegen()
    {
        if (currentStamina >= maxStamina) return;

        if (playerMovement.IsRunning) return;
        
        if (regenTimer > 0f)
        {
            regenTimer -= Time.deltaTime;
            return;
        }
            
        if(currentStamina < maxStamina)
        {
            currentStamina += StaminaRegenRate * Time.deltaTime;
            currentStamina = Mathf.Clamp(currentStamina, 0, maxStamina);
        }
        
    }
}
