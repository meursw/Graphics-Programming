#[compute]
#version 450

#define PI 3.14159265358979323846

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(set = 0, binding = 0, std430) restrict buffer MyDataBuffer {
    vec3 Positions[];
}
CB; // compute buffer

layout(set = 0, binding = 1) uniform MyUniformBuffer {
    float Step, Time, Resolution, TransitionProgress;
}
UB; // uniform buffer

layout(set = 1, binding = 0, rgba32f) uniform image2D positionsTexture;

vec2 GetUV(uvec3 id) {
    return (vec2(id.xy) + 0.5) * UB.Step - 1.0;
}

void SetPosition(uvec3 id, vec3 position) {
    if (id.x < uint(UB.Resolution) && id.y < uint(UB.Resolution)) {
        CB.Positions[id.x + id.y * uint(UB.Resolution)] = position;
        imageStore(positionsTexture, ivec2(id.xy), vec4(position, 1.0));
    }
}

vec3 Wave (float u, float v, float t) {
	vec3 p;
	p.x = u;
	p.y = sin(PI * (u + v + t));
	p.z = v;
	return p;
}

void main() {
    vec2 uv = GetUV(gl_GlobalInvocationID);
    SetPosition(gl_GlobalInvocationID, Wave(uv.x, uv.y, UB.Time));
}