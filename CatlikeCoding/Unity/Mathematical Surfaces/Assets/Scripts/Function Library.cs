using UnityEngine;
using static UnityEngine.Mathf;

public static class FunctionLibrary {
    public delegate Vector3 Function (float u, float v, float t);
    static Function[] functions = {Wave, MultiWave, Ripple};

    public enum FunctionName {Wave, MultiWave, Ripple};

    public static Function GetFunction(FunctionName name) {
        return functions[(int)name];
    }

    public static Vector3 Wave(float u, float v, float t) {
        Vector3 p = Vector3.zero;
        p.x = u;
        return p;
    }

    public static Vector3 MultiWave(float u, float v, float t) {
        Vector3 p = Vector3.zero;
        return p;
    }

    public static Vector3 Ripple(float u, float v, float t) {
        Vector3 p = Vector3.zero;
        return p;
    }
}