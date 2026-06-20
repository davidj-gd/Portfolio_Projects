using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerMovement : MonoBehaviour
{
    [Header("Movement Settings")]
    [SerializeField] float moveSpeed = 2f;

    private Vec2 position;
    private Vec2 direction;
    private Vec2 movement;
    private float input = 0f;

    private void Start()
    {
        position.x = transform.position.x;
        position.y = transform.position.y;
    }

    private void Update()
    {
        OnMoveP1();
    }

    private void OnMoveP1()
    {
        if (Keyboard.current.wKey.isPressed)
        {
            input = 1f;
            HandleMove();
        }

        if (Keyboard.current.sKey.isPressed)
        {
            input = -1f;
            HandleMove();
        }
    }

    private void HandleMove()
    {
        direction.x = 0f;
        direction.y = input;

        float distance = moveSpeed * Time.deltaTime;

        movement = VectorMath.Vector2Scale(direction, distance);

        position = VectorMath.Vector2Add(position, movement);

        transform.position = new Vector3(position.x, position.y, transform.position.z);

    }

}
