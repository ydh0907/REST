using UnityEngine;

public class PlayerMovement : MonoBehaviour
{
    private new Rigidbody2D rigidbody;

    [SerializeField] private float torquePower = 100;

    private void Awake()
    {
        rigidbody = GetComponent<Rigidbody2D>();
    }

    private void Update()
    {
        float x = Input.GetAxis("Horizontal");
        rigidbody.AddTorque(-x * torquePower);
    }
}
