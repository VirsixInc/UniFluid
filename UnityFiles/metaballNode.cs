using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class metaballNode : MonoBehaviour
{
    public metaballMeshGenerator meshGenerator;
    public float charge = 0.5f;

    void Start()
    {
        meshGenerator = (metaballMeshGenerator)(
            gameObject.transform.parent.gameObject.GetComponent(typeof(metaballMeshGenerator)));
        meshGenerator.addChargeNode(gameObject);
    }

    void Disable()
    {
        meshGenerator.removeChargeNode(gameObject);
    }
}