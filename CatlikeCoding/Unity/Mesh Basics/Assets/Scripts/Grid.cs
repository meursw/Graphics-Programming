using System.Collections;
using UnityEngine;

[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class Grid : MonoBehaviour {
    public int xSize, ySize;

    private Vector3[] vertices;

    void Awake() {
        Generate();
    }

    private Mesh mesh;

    private void Generate() {
        WaitForSeconds wait = new WaitForSeconds(0.05f);

        GetComponent<MeshFilter>().mesh = mesh = new Mesh();
        mesh.name = "Procedural Grid";

        vertices = new Vector3[(xSize+1)*(ySize+1)];
        Vector2[] uv = new Vector2[vertices.Length];
        Vector4[] tangents = new Vector4[vertices.Length];
        Vector4 tangent = new Vector4(1f,0f,0f,-1f);
        int i = 0;
        for(int y = 0; y <= ySize; y++) {
            for(int x = 0; x <= xSize; x++, i++) {
                vertices[i] = new Vector3(x, y);
                uv[i] = new Vector2((float)x / xSize, (float)y / ySize);
                tangents[i] = tangent;
            }
        }

        mesh.vertices = vertices;
        mesh.uv = uv;
        mesh.tangents = tangents;

        int[] triangles = new int[6*xSize*ySize];

        for(int ayi = 0, vi = 0, y = 0; y < ySize; ayi += 6*xSize, vi++, y++) {
            for(int ai = 0, x = 0; x < xSize; ai += 6, vi++, x++) {
                triangles[ai + 0 + ayi] = vi;
                triangles[ai+ 1 + ayi] = triangles[ai + 4+ ayi] = xSize + 1 + vi;
                triangles[ai + 2 + ayi] = triangles[ai + 3+ ayi] = 1 + vi;
                triangles[ai+ 5 + ayi] = xSize + 2 + vi; 
            }
        }
        mesh.triangles = triangles;
        mesh.RecalculateNormals();
    }

    private void OnDrawGizmos() {
        if(vertices == null)
            return;
        Gizmos.color = Color.black;
        for(int i = 0; i < vertices.Length; i++) {
            Gizmos.DrawSphere(transform.TransformPoint(vertices[i]), 0.1f);
        }
    }

}
