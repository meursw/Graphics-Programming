#[compute]
#version 450

#define PI 3.14159265358979323846

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(set = 0, binding = 0, std430) restrict buffer MyDataBuffer {
    vec3 _Positions[];
};

layout(set = 0, binding = 1, std430) restrict buffer MyUniformBuffer {
    float Step, Time, Resolution;
}
UB;

vec2 GetUV(uvec3 id) {
    return (vec2(id.xy) + 0.5) * UN.Step - 1.0;
}

void SetPosition(uvec3 id, vec3 position) {
    if (id.x < _Resolution && id.y < UB.Resolution) {
        _Positions[id.x + id.y * UB.Resolution] = position;
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