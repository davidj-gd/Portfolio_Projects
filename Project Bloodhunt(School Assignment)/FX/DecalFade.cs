// DecalFade.cs
using UnityEngine;
using System.Collections;
using UnityEngine.Rendering.HighDefinition;

public class DecalFade : MonoBehaviour
{
    private DecalProjector projector;
    private Material mat;

    private void Start()
    {
        projector = GetComponent<DecalProjector>();
        if (projector == null)
        {
            Debug.LogError("DecalFade: No DecalProjector found.");
            return;
        }

        // HDRP creates the material instance after Awake; Start is safe
        mat = projector.material;
    }

    public void BeginFade(float lifetime)
    {
        StartCoroutine(FadeRoutine(lifetime));
    }

    private IEnumerator FadeRoutine(float lifetime)
    {
        float t = 0f;
        while (t < lifetime)
        {
            t += Time.deltaTime;
            float a = Mathf.Clamp01(1f - (t / lifetime));
            if (mat != null)
                mat.SetFloat("_Opacity", a);
            yield return null;
        }
        Destroy(gameObject);
    }
}