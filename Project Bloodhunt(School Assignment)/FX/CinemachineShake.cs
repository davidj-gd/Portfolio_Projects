using UnityEngine;
using Unity.Cinemachine;
public class CinemachineShake : MonoBehaviour
{
    public static CinemachineShake Instance;

    [SerializeField] private float globalShakeForce = 1f;

    private void Awake()
    {
        Instance = this;
    }

    public void cameraShake(CinemachineImpulseSource impulseSource)
    {
        impulseSource.GenerateImpulse(globalShakeForce);
    }
}
