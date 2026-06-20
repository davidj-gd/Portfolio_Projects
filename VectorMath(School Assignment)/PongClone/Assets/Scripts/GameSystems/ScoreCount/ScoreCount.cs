using System;
using UnityEngine;
using TMPro;

public class ScoreCount : MonoBehaviour
{
    [Header("Player 1")]
    [SerializeField] TextMeshProUGUI player1ScoreText;
    public int player1score;
    public bool p1Wins = false;
    
    [Header("Player 2")]
    [SerializeField] TextMeshProUGUI player2ScoreText;
    public int player2score;
    public bool p2Wins = false;
    
    [Header("Ball Reference")]
    [SerializeField] BallMovement ball;
    
    [Header("Game System Reference")]
    [SerializeField] GameSystem gameSystem;

    public int maxScore = 5;

    public void Start()
    {
        ResetScore();
    }

    public void Update()
    {
        CheckWin();
    }

    public void UpdatePlayer1Score()
    {
        player1score++;
        UpdateScoreUI();
        ball.StartResetAfterScore();
    }
    
    public void UpdatePlayer2Score()
    {
        player2score++;
        UpdateScoreUI();
        if (player1score < maxScore && player2score < maxScore)
        {
            ball.StartResetAfterScore();
        }
    }

    public void UpdateScoreUI()
    {
        player1ScoreText.text = player1score.ToString();
        player2ScoreText.text = player2score.ToString();
    }

    public void ResetScore()
    {
        player1score = 0;
        player2score = 0;
        p1Wins = false;
        p2Wins = false;
        UpdateScoreUI();
    }

    public void CheckWin()
    {
        if (player1score >= maxScore && !p1Wins)
        {
            p1Wins = true;
            gameSystem.GameOver();
        }

        if (player2score >= maxScore && !p2Wins)
        {
            p2Wins = true;
            gameSystem.GameOver();
        }
    }
}
