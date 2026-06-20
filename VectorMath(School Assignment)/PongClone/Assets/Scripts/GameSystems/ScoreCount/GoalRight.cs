using TMPro;
using UnityEngine;

public class GoalRight : MonoBehaviour
{
    [SerializeField] private GameObject ball;
    [SerializeField] private ScoreCount scoreCount;

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject == ball)
        {
            scoreCount.UpdatePlayer1Score();
        }
        
    }
    
}
