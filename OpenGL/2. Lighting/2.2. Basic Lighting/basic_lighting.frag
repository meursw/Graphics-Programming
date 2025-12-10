#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);
    float lightIntensity = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = lightIntensity * lightColor;

    // Specular 
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectionDir = -lightDirection - 2.0*max(dot(normal,-lightDirection), 0.0) * normal;
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;


    vec3 result = (ambient+diffuse+specular) * objectColor;

    FragColor = vec4(result, 1.0);
}