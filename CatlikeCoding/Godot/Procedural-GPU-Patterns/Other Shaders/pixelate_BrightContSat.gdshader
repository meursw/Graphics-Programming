shader_type canvas_item;

uniform sampler2D screen_texture : hint_screen_texture, repeat_disable, filter_nearest;

uniform float brightness : hint_range(0.0, 2.0, 0.1) = 1.0;
uniform float contrast : hint_range(0.0, 2.0, 0.1) = 1.0; 
uniform float saturation : hint_range(0.0, 2.0, 0.1) = 1.0;

uniform float pixel_size : hint_range(0.01, 150.0, 2.0) = 4.0;

vec3 pixelate(vec2 screen_uv) {
	vec2 tex_size = vec2(textureSize(screen_texture, 0));
	vec2 pixel_count = tex_size / pixel_size;
	vec2 pixelated_uv = (floor(screen_uv * pixel_count) + 0.5 )/ pixel_count;
	return texture(screen_texture, pixelated_uv).rgb;
}

vec3 screen_adjustments(vec3 c) {
	c.rgb = mix(vec3(0.0), c.rgb, brightness);
	c.rgb = mix(vec3(0.5), c.rgb, contrast);
	
	float grayscale = dot(vec3(0.2126, 0.7152, 0.0722), c.rgb);
	c.rgb = mix(
				vec3(grayscale), 
				c.rgb, 
				saturation
			);
	
	return c;
}

void fragment() {
	vec3 c = pixelate(SCREEN_UV);
	c = screen_adjustments(c); // Manipulates brightness, contrast and satuartion
	
	COLOR.rgb = c;
}
