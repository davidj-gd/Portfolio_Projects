using Unity.VectorGraphics;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.SceneManagement;
using TMPro;

public class GameSystem : MonoBehaviour
{
    
    [SerializeField] ScoreCount scoreCount;
    [SerializeField] private GameObject gameOverPanel;
    [SerializeField] private TMP_Text gameOverText;
    [SerializeField] private BallMovement ball;
    

    private bool gameOver = false;

    private void Start()
    {
        gameOverPanel.SetActive(false);
    }

    private void Update()
    {
        if (gameOver)
        {
            if (Keyboard.current.rKey.wasPressedThisFrame)
            {
                NewGame();
            }
        }
    }

    public void NewGame()
    {
        gameOver = false;
        scoreCount.ResetScore();
        gameOverPanel.SetActive(false);
        ball.countdownPanel.SetActive(false);
        ball.StartResetAfterScore();
        
    }

    public void OnStartGame()
    {
        SceneManager.LoadScene("PongScene");
        gameOverPanel.SetActive(false);
        NewGame();
    }

    public void GameOver()
    {
        gameOverPanel.SetActive(true);
        ball.countdownPanel.SetActive(false);
        gameOver = true;
        ball.FreezeBall();

        if (scoreCount.p1Wins)
        {
            gameOverText.text = "Congratulations, Player 1 wins";
        }
        else
        {
            gameOverText.text = "Congratulations, Player 2 wins";
        }
    }

    public void OnMainMenu()
    {
        SceneManager.LoadScene("MainMenuScene");
    }

    public void OnQuitGame()
    {
        Application.Quit();
    }
}
