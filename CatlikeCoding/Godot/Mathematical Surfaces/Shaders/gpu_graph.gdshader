shader_type spatial;

uniform sampler2D positions_texture;

void vertex() {
	vec4 position_data = texture(positions_texture, UV);
	VERTEX = position_data.xyz;
}
