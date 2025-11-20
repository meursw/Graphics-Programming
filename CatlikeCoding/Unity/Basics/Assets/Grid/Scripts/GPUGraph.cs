using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GPUGraph : MonoBehaviour
{
    ComputeBuffer positionsBuffer;
    [SerializeField] ComputeShader computeShader;
    static readonly int positionsId = Shader.PropertyToID("_Positions");
    static readonly int resolutionId = Shader.PropertyToID("_Resolution");
    static readonly int stepId = Shader.PropertyToID("_Step");
    static readonly int timeId = Shader.PropertyToID("_Time");
    static readonly int transitionProgressId = Shader.PropertyToID("_TransitionProgress");

    [SerializeField] Material material;
    [SerializeField] Mesh mesh;

    const int maxResolution = 1000;
    [SerializeField, Range(10, maxResolution)] int resolution = 200;
    [SerializeField] FunctionLibrary.FunctionName function;
    public enum TransitionMode { Cycle, Random }
    [SerializeField] TransitionMode transitionMode;
    [SerializeField, Min(0f)] float transitionDuration = 1f;
    bool transitioning = false;
    FunctionLibrary.FunctionName transitionFunction;

    [SerializeField, Min(0f)] float functionDuration = 1f;
    float duration = 0f;

    private void OnEnable()
    {
        positionsBuffer = new ComputeBuffer(maxResolution * maxResolution, 3*4);
    }
    private void OnDisable()
    {
        positionsBuffer.Release();
        positionsBuffer = null;
    }
    private void UpdateFunctionOnGPU()
    {
        float step = 2f / resolution;
        computeShader.SetInt(resolutionId, resolution);
        computeShader.SetFloat(stepId, step);
        computeShader.SetFloat(timeId, Time.time);
        if (transitioning)
        {
            computeShader.SetFloat(
                transitionProgressId,
                Mathf.SmoothStep(0f, 1f, duration / transitionDuration)
            );
        }

        var kernelIndex =
             (int)function + (int)(transitioning ? transitionFunction : function) * FunctionLibrary.FunctionCount; ;
        computeShader.SetBuffer(kernelIndex, positionsId, positionsBuffer);
        int groups = Mathf.CeilToInt(resolution / 8f);
        computeShader.Dispatch(kernelIndex, groups, groups, 1);

        material.SetBuffer(positionsId, positionsBuffer);
        material.SetFloat(stepId, step);
        var bounds = new Bounds(Vector3.zero, Vector3.one * (2f + 2f / resolution));
        Graphics.DrawMeshInstancedProcedural(mesh, 0, material, bounds, resolution*resolution);
    }

    private void Update()
    {
        duration += Time.deltaTime;
        if (transitioning && duration >= transitionDuration)
        {
            duration -= transitionDuration;
            transitioning = false;
        }
        else if (duration >= functionDuration) { 
            duration -= functionDuration;
            transitioning = true;
            transitionFunction = function;
            PickNextFunction();
        }

        UpdateFunctionOnGPU();
    }

    void PickNextFunction()
    {
        if (transitionMode == TransitionMode.Cycle) {
            function = FunctionLibrary.GetNextFunctionName(function);
        }
        else
        {
            function = FunctionLibrary.GetRandomFunctionNameOtherThan(function);
        }
    }
}