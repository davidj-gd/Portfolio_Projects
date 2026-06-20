using SoulsLikeProject.Player.Temp;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerInteract : MonoBehaviour
{
    [SerializeField] float interactRange = 2.5f;
    [SerializeField] LayerMask InteractableObject;
    [SerializeField] PlayerMovement playerMovement;

    public bool isInteracting = false;

    private Interactable current;

    public void OnInteract(InputAction.CallbackContext context)
    {
        if (!context.performed) return;

        TryInteract();
    }

    private void TryInteract()
    {
        Collider[] closeInteractable = Physics.OverlapSphere(transform.position, interactRange, InteractableObject);

        float closestDist = Mathf.Infinity;
        Interactable best = null;

        foreach (var hit in closeInteractable)
        {
            if (hit.TryGetComponent(out Interactable interactable))
            {
                float dist = Vector3.Distance(transform.position, hit.transform.position);

                if (dist < closestDist)
                {
                    closestDist = dist;
                    best = interactable;
                }
            }
        }

        if (best != null)
        {
            best.Interact();
        }
    }

}
