using UnityEngine;
using static UnityEngine.Mathf;

public static class FunctionLibrary {
    public delegate Vector3 Function (float u, float v, float t);
    static Function[] functions = {Wave, MultiWave, Ripple};

    public enum FunctionName {Wave, MultiWave, Ripple};

    public static Function GetFunction(FunctionName name) {
        return functions[(int)name];
    }

    public static Vector3 Wave(float x, float z, float t) {
        Vector3 p;
        p.x = u;
        return p;
    }

    public static Vector3 MultiWave(float x, float z, float t) {
        Vector3 p;
        return p;
    }

    public static Vector3 Ripple(float x, float z, float t) {
        Vector3 p;
        return p;
    }
}