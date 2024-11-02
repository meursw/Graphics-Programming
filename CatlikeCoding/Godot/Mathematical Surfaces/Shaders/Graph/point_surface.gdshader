shader_type spatial;
render_mode world_vertex_coords;

void vertex() {

}

void fragment() {
	ALBEDO = clamp(VERTEX * 0.5 + 0.5, 0.5, 1.0);
}
