using UnityEngine;

public class PlayerCombatDamageText : MonoBehaviour
{
    [SerializeField] private TextMesh damageTextMesh;
    public Vector3 Offset = new Vector3(0, 1, 0);

    public void SetText(string text)
    {
        if(damageTextMesh != null)
        {
            damageTextMesh.text = text;

            transform.localPosition += Offset;
        }
        else
        {
            Debug.LogWarning("Damage Text Mesh is not assigned.");
        }
    }

    public void SetColor(Color color)
    {
        if(damageTextMesh != null)
        {
            damageTextMesh.color = color;
        }
        else
        {
            Debug.LogWarning("Damage Text Mesh is not assigned.");
        }
    }

    private void LateUpdate()
    {
        transform.forward = Camera.main.transform.forward;
    }
}





