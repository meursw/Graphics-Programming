#version 330 core
struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuseMap, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float lightIntensity = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * lightIntensity * texture(material.diffuseMap, TexCoords).rgb;

    // specular
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflection = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDirection, reflection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specularMap, TexCoords).rgb;

    // emission Map
    vec3 emission = vec3(0.0f);
    if(texture(material.specularMap, TexCoords).r == 0.0) {
        emission = texture(material.emissionMap, TexCoords).rgb;
    }

    vec3 result = ambient + diffuse + specular + emission;

    FragColor = vec4(result, 1.0);
}