using Unity.VisualScripting;
using UnityEngine;
using System.Collections;
using TMPro;
using UnityEngine.UIElements;

public class BallMovement : MonoBehaviour
{
    [Header("Ball Settings")]
    [SerializeField] float ballSpeed = 5f;
    [SerializeField] float ballResetTime = 3f;
    
    [SerializeField] TMP_Text countdownText;
    [SerializeField] public GameObject countdownPanel;

    private Vec2 position;
    private Vec2 velocity;
    private Rigidbody2D rb;
    private Vec2 startPosition;
    private float countdown = 1f;
    
    public bool hasRestarted = false;

    private void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        
        startPosition.x = rb.position.x;
        startPosition.y = rb.position.y;
        
        position = startPosition;
        
        countdownPanel.SetActive(false);
        StartResetAfterScore();
        
        hasRestarted = false;

    }

    private void FixedUpdate()
    {
        Vec2 move = VectorMath.Vector2Scale(velocity, Time.fixedDeltaTime);
        position = VectorMath.Vector2Add(position, move);
        
        rb.MovePosition(new Vector2(position.x, position.y));
    }

    public void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.contactCount == 0)
        {
            return;
        }

        Vector2 norm = collision.contacts[0].normal;

        Vec2 normal;
        normal.x = norm.x;
        normal.y = norm.y;
        
        velocity = VectorMath.Vector2Reflect(velocity, normal);
        velocity = VectorMath.Vector2Normalize(velocity);
        velocity = VectorMath.Vector2Scale(velocity, ballSpeed);
    }

    public void StartResetAfterScore()
    {
        if (!hasRestarted)
        {
            StartCoroutine(ResetBall());
            StartCoroutine(Countdown());
            hasRestarted = true;
        }
    }

    private IEnumerator ResetBall()
    {
        yield return new WaitForSeconds(ballResetTime);
        
        velocity.x = 0f;
        velocity.y = 0f;
        rb.linearVelocity = Vector2.zero;

        hasRestarted = false;

        position = startPosition;
        rb.position = new Vector2(position.x, position.y);

        float rnd = Random.value < 0.5f ? -1f : 1f;
        velocity.x = rnd * 1f;
        velocity.y = Random.Range(-0.2f, 0.2f);
        
        velocity = VectorMath.Vector2Normalize(velocity);
        velocity = VectorMath.Vector2Scale(velocity, ballSpeed);
    }

    private IEnumerator Countdown()
    {
        countdownPanel.SetActive(true);
        countdownText.text = "3";
        yield return new WaitForSeconds(countdown);
        countdownText.text = "2";
        yield return new WaitForSeconds(countdown);
        countdownText.text = "1";
        yield return new WaitForSeconds(countdown);
        countdownPanel.SetActive(false);
    }

    public void FreezeBall()
    {
        velocity.x = 0f;
        velocity.y = 0f;
        rb.linearVelocity = Vector2.zero;
    }
}
