#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
	vec4 tex = texture(texture1, TexCoords);
	float depth_value = LinearizeDepth(gl_FragCoord.z) / far;

	FragColor =  vec4(vec3(depth_value), 1.0);

}