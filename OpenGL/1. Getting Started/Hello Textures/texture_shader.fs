#version 330 core

in vec2 TexCoord;
in vec4 outColor;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float u_mix = 0.5;

void main() {
    // Sample the colors from both textures
    vec4 tex1Color = texture(texture1, TexCoord);
    vec4 tex2Color = texture(texture2, TexCoord);

    // Adjust the mixing based on tex2Color's alpha
    FragColor = mix(tex1Color, tex2Color, tex2Color.a * u_mix) * outColor;
}
