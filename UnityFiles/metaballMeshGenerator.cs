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
    private static extern void updateLattice(int latticeID);
    [DllImport("mbUnity")]
    unsafe private static extern float* getLatticeFrameVertices(int latticeID);
    [DllImport("mbUnity")]
    unsafe private static extern float* getLatticeFrameNormals(int latticeID);
    [DllImport("mbUnity")]
    unsafe private static extern int* getLatticeFrameTriangles(int latticeID);
    [DllImport("mbUnity")]
    private static extern int getLatticeFrameVertCount(int latticeID);
    [DllImport("mbUnity")]
    private static extern int getLatticeFrameTriCount(int latticeID);

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
        latticeID = setupLattice(10, 10, 10, 1.0f, 1.0f, 1.0f);
    }

    void startEngine()
    {
        ((MeshFilter)GetComponent("MeshFilter")).mesh = new Mesh();
    }

    // Update is called once per frame
    public Vector3[] vertices, normals;
    public int[] triangles;
    private int lastVertexCount = -1;
    private int lastTriangleCount = -1;

    void Update()
    {
        int i;
        Vector3 position;
        float charge;
        GameObject nodeObject;
        clearLatticeCharges(latticeID);
        //Debug.Log(nodeObjects.Count);
        for (i = 0; i < nodeObjects.Count; i++)
        {
            nodeObject = nodeObjects[i];
            position = nodeObject.transform.position;
            charge = ((metaballNode)nodeObject.GetComponent(typeof(metaballNode))).charge;
            addChargeToLattice(latticeID, position.x, position.y, position.z, charge);
        }
        updateLattice(latticeID);
        Mesh mesh = ((MeshFilter)GetComponent("MeshFilter")).mesh;
        int vertexCount = getLatticeFrameVertCount(latticeID);
        int triangleCount = getLatticeFrameTriCount(latticeID);
        if (vertexCount != lastVertexCount)
        {
            vertices = new Vector3[vertexCount];
            normals = new Vector3[vertexCount];
            lastVertexCount = vertexCount;
        };
        if (triangleCount != lastTriangleCount)
        {
            triangles = new int[triangleCount];
            lastTriangleCount = triangleCount;
        }
        int actualIndex;
        unsafe
        {
            float* verticesData = getLatticeFrameVertices(latticeID);
            float* normalsData = getLatticeFrameNormals(latticeID);
            int* trianglesData = getLatticeFrameTriangles(latticeID);
            for (i = 0; i < vertexCount; i++)
            {
                actualIndex = i * 3;
                //vertices[i] = new Vector3(verticesData[actualIndex], 
                //    verticesData[actualIndex+1], verticesData[actualIndex+2]);
                //normals[i] = new Vector3(normalsData[actualIndex],
                //    normalsData[actualIndex + 1], normalsData[actualIndex + 2]);
                vertices[i].x = verticesData[actualIndex];
                vertices[i].y = verticesData[actualIndex + 1];
                vertices[i].z = verticesData[actualIndex + 2];
                normals[i].x = normalsData[actualIndex];
                normals[i].y = normalsData[actualIndex + 1];
                normals[i].z = normalsData[actualIndex + 2];
            };
            for (i = 0; i < triangleCount; i++)
            {
                triangles[i] = trianglesData[i];
            };
        };
        //Debug.Log(vertexCount);
        mesh.Clear();
        mesh.vertices = vertices;
        mesh.triangles = triangles;
        mesh.normals = normals;

    }

    void OnDisable()
    {
        removeLattice(latticeID);
        latticeID = -1;
    }
}
