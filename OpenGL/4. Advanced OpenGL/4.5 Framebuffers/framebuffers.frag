#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D renderTexture;

const float offset = 1.0 / 300.0;

void main()
{
	
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset), vec2(0.0f, offset), vec2(offset, offset),

		vec2(-offset, 0.0), vec2(0.0, 0.0), vec2(offset, 0.0),

		vec2(-offset, -offset), vec2(0.0f, -offset), vec2(offset, -offset)
	);

	float kernel[9] = float[](
		-2.0, -1.0, 0.0,
		-1.0, 1.0, 1.0,
		0.0, 1.0, 2.0
	);

	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++) {
		vec2 uv = TexCoords + offsets[i];
		col += texture(renderTexture, uv).rgb * kernel[i];
	}

	FragColor = vec4(col, 1.0);
}