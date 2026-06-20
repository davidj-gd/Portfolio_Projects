using UnityEngine;
using TMPro;

public class GoalLeft : MonoBehaviour
{
    [SerializeField] private GameObject ball;
    [SerializeField] private ScoreCount scoreCount;

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject == ball)
        {
            scoreCount.UpdatePlayer2Score();
        }
        
    }
    
}
