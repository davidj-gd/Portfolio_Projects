using UnityEngine;

public class WeaponFX : MonoBehaviour
{
    [SerializeField] private ParticleSystem attackParticles;

    public void PlayAttackVFX()
    {
        if (attackParticles == null) return;
        if (!attackParticles.isPlaying)
            attackParticles.Play();
    }

    public void StopAttackVFX()
    {
        if (attackParticles == null) return;
        if (attackParticles.isPlaying)
            attackParticles.Stop();
    }

}
