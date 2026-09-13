#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	vec3 viewDir = normalize(Position - cameraPos);
	vec3 reflectVec = reflect(viewDir, normalize(Normal));

	FragColor = vec4(texture(skybox, reflectVec).rgb, 1.0);

}