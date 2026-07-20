shader_type spatial;

uniform vec2 animation_speed = vec2(0.0);
uniform vec2 bumpiness = vec2(1.0);
uniform float displacement: hint_range(-1.0, 1.0) = 0.2;
uniform vec2 frequency = vec2(1.0);

uniform int function : hint_enum("U", "V", "UV Average", "UV_PRODUCT") = 0;
uniform bool colors_per_vertex = false;
uniform bool derivatives_per_vertex = false;
uniform bool vertex_displacement = true;

varying vec2 derivatives;

struct PatternSample {
	float v;
	vec2 d;
};

vec3 colorize(float v) {
	if(v >= 0.0)
		return vec3(v);
	return vec3(0.0, -v, 0.0);
}

PatternSample sample_pattern(vec2 uv) {
	vec2 f = frequency * TAU;
	vec2 t = f * uv;
	PatternSample s;
	
	switch(function) {
		case 0:
			s.v = sin(t.x);
			s.d.x = f.x*cos(t.x);
			s.d.y = 0.0;
			break;
		case 1:
			s.v = sin(t.y);
			s.d.x = 0.0;
			s.d.y = f.y*cos(t.y);
			break;
		case 2:
			s.v = (sin(t.x) + sin(t.y)) * 0.5;
			s.d.x = f.x*cos(t.x)*0.5;
			s.d.y = f.y*cos(t.y)*0.5;
			break;
		case 3:
			s.v = (sin(t.x) * sin(t.y));
			s.d.x = f.x * cos(t.x) * sin(t.y);
			s.d.y = f.y * sin(t.x) * cos(t.y);
			break;
	}
	
	return s;
}

void vertex() {
	PatternSample pattern_sample = sample_pattern(UV + animation_speed * TIME);
	
	if(colors_per_vertex) {
		COLOR.rgb = colorize(pattern_sample.v);
	}
	
	if(vertex_displacement)
		VERTEX += NORMAL * (pattern_sample.v * displacement);
	
	if (derivatives_per_vertex) {
		derivatives = pattern_sample.d * displacement * bumpiness;
	}
}

void fragment() {
	PatternSample pattern_sample = sample_pattern(UV + animation_speed * TIME);
	if(colors_per_vertex){
		ALBEDO = COLOR.rgb;
	} else {
		ALBEDO = colorize(pattern_sample.v);
	}
	
	vec2 d;
	if (derivatives_per_vertex) {
		d = derivatives;
	}
	else {
		d = pattern_sample.d * displacement * bumpiness;
	}
	
	NORMAL = cross(
		TANGENT + NORMAL * d.x,
		BINORMAL - NORMAL * d.y
	);
}