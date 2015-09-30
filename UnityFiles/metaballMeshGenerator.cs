using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class metaballMeshGenerator : MonoBehaviour
{
#if UNITY_IPHONE || UNITY_XBOX360
   
    // On iOS and Xbox 360 plugins are statically linked into
    // the executable, so we have to use __Internal as the
    // library name.
    [DllImport ("__Internal")]
    private static extern void setupLattice(uint xDim, uint yDim, uint zDim, float xScale, float yScale, float zScale);


#else

    // Other platforms load plugins dynamically, so pass the name
    // of the plugin's dynamic library.
    [DllImport("mbUnity")]
    private static extern int setupLattice(uint xDim, uint yDim, uint zDim, float xScale, float yScale, float zScale);
    [DllImport("mbUnity")]
    private static extern void removeLattice(int latticeId);
    [DllImport("mbUnity")]
    private static extern void setLatticeXDim(int latticeID, uint xDim);
    [DllImport("mbUnity")]
    private static extern void setLatticeYDim(int latticeID, uint yDim);
    [DllImport("mbUnity")]
    private static extern void setLatticeZDim(int latticeID, uint zDim);
    [DllImport("mbUnity")]
    private static extern uint getLatticeXDim(int latticeID);
    [DllImport("mbUnity")]
    private static extern uint getLatticeYDim(int latticeID);
    [DllImport("mbUnity")]
    private static extern uint getLatticeZDim(int latticeID);
    [DllImport("mbUnity")]
    private static extern void addChargeToLattice(int latticeID, float x, float y, float z, float charge);
    [DllImport("mbUnity")]
    unsafe private static extern float* getLatticeCharges(int latticeID);
    [DllImport("mbUnity")]
    unsafe private static extern float* getChargeLattice(int latticeID);
    [DllImport("mbUnity")]
    private static extern uint getCurrentLatticeSize(int latticeID);
    [DllImport("mbUnity")]
    private static extern void clearLatticeCharges(int latticeID);
    [DllImport("mbUnity")]
    private static extern void deallocLatticeChargeReturn(int latticeID);
    [DllImport("mbUnity")]
    private static extern void updateCharge(int latticeID);

#endif
    private int latticeID = -1;
    private List<GameObject> nodeObjects = new List<GameObject>();
    public uint xDim
    {
        get { return getLatticeXDim(latticeID); }
        set { setLatticeXDim(latticeID, value); }
    }
    public uint yDim
    {
        get { return getLatticeYDim(latticeID); }
        set { setLatticeYDim(latticeID, value); }
    }
    public uint zDim
    {
        get { return getLatticeZDim(latticeID); }
        set { setLatticeZDim(latticeID, value); }
    }


    public void addChargeNode(GameObject node)
    {
        nodeObjects.Add(node);
    }

    public void removeChargeNode(GameObject node)
    {
        nodeObjects.Remove(node);
    }

    void Start()
    {
        latticeID = setupLattice(64, 64, 64, 1.0f, 1.0f, 1.0f);
    }

    // Update is called once per frame
    void Update()
    {
        int i;
        Vector3 position;
        float charge;
        GameObject nodeObject;
        clearLatticeCharges(latticeID);
        for (i = 0; i < nodeObjects.Count; i++)
        {
            nodeObject = nodeObjects[i];
            position = nodeObject.transform.position;
            charge = ((metaballNode)nodeObject.GetComponent(typeof(metaballNode))).charge;
            addChargeToLattice(latticeID, position.x, position.y, position.z, charge);
        }
        updateCharge(latticeID);
    }

    void OnDisable()
    {
        removeLattice(latticeID);
        latticeID = -1;
    }
}
