#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 lightDirection;
uniform vec3 lightDirection2;

uniform sampler2D texture_diffuse1;

vec3 CalcDirLight(vec3 dir, vec3 normal, vec3 viewDir);

void main()
{   
    vec3 result = vec3(0.0f);

    result += CalcDirLight(lightDirection, normalize(Normal), normalize(viewPos - FragPos));
    result += CalcDirLight(lightDirection2, normalize(Normal), normalize(viewPos - FragPos)); 

    FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(vec3 lightDirection, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-lightDirection);    
    float lightIntensity = max(dot(normal, lightDir), 0.0);

    vec3 lightColor;
    if(lightDirection.y == -1.0)
        lightColor = vec3(1.0f);
    else
        lightColor = vec3(253.0/255.0, 94.0/255.0, 83.0/255.0);
    
    vec3 ambient = 0.1 * lightColor; 
    vec3 diffuse = 0.8 * lightIntensity * lightColor; 

    return (ambient + diffuse);
}