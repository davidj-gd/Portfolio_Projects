using System;
using SoulsLikeProject.Player.Temp;
using UnityEngine;
using System.Collections;
using SoulsLikeProject.Quests;

public class PlayerRespawn : MonoBehaviour
{
    [SerializeField] PlayerHealth playerHealth;
    [SerializeField] Transform spawnPoint;
    [SerializeField] GameObject playerPrefab;
    [SerializeField] PlayerHealthbar healthbar;
    [SerializeField] private QuestManager questManager;
    
    private bool questHasFailed = false;
    private float respawndelay = 2f;

    private void Awake()
    {
        if(playerHealth == null)
        {
            playerHealth = GetComponent<PlayerHealth>();
        }

        if(healthbar == null)
        {
            healthbar = GetComponent<PlayerHealthbar>();
        }
        
        if (questManager == null)
        {
            questManager = QuestManager.Instance;
        }
    }

    private void Start()
    {
        if (playerHealth != null)
        {
            playerHealth.OnDeath += HandleRespawn;
        }
        else
        {
            Debug.LogError("No PlayerHealth component found on " + gameObject.name);
        }

        if (questManager != null)
        {
            questManager.OnQuestFailed += HandleQuestFailed;
        }
        else
        {
            Debug.LogWarning("[PlayerRespawn] QuestManager not found - respawn will always happen");
        }
    }

    private bool CanRespawn()
    {
        if (questManager == null)
        {
            return true;
        }
        
        if (questManager.CurrentState == QuestState.QuestFailed)
        {
            return false;
        }
        
        QuestState state = questManager.CurrentState;
        return state == QuestState.InQuest || !questManager.IsQuestActive;

    }

    private void HandleRespawn()
    {
        if (questHasFailed)
        {
            Debug.LogError("[PlayerRespawn] Quest has failed - respawn blocked");
            return;
        }
        
        if (!CanRespawn())
        {
            Debug.Log("[PlayerRespawn] Cannot respawn - quest state doesn't allow it");
            return;
        }
        
        StartCoroutine(WaitForRespawn());
    }

    private void HandleQuestFailed()
    {
        questHasFailed = true;
        Debug.Log("[PlayerRespawn] Quest failed - blocking all future respawns");
        StopAllCoroutines();
    }



    #region Coroutine

    private IEnumerator WaitForRespawn()
    {
        yield return new WaitForSeconds(respawndelay);

        

        if (spawnPoint != null)
        {
            Vector3 position = spawnPoint.transform.position;

            position.y += 0.5f;
            position.z += 0.5f;

            playerPrefab.transform.position = position;

        }

        playerHealth.ResetHealth();
        healthbar.SetHealth(playerHealth.CurrentHealth);
    }

    #endregion

    private void OnDestroy()
    {
        if (playerHealth != null)
        {
            playerHealth.OnDeath -= HandleRespawn;
        }

        if (questManager != null)
        {
            questManager.OnQuestFailed -= HandleQuestFailed;
        }
    }
}
