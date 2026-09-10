shader_type spatial;

uniform uint hash_seed = 0u;

uniform bool animation = true;
uniform bool colored = true;
uniform vec3 color_min : source_color = vec3(1.0, 0.0, 0.0);
uniform vec3 color_mid : source_color = vec3(0.0);
uniform vec3 color_max : source_color = vec3(1.0);

#define USE_STANDARD_FRACTAL_PATTERN_SETTINGS
#define USE_FRACTAL_PATTERN_ANIMATION_SETTINGS
#include "res://Shader Library/fractal_pattern_settings.gdshaderinc"

#include "res://Shader Library/pattern_sample.gdshaderinc"
#include "res://Shader Library/hasher.gdshaderinc"

// Mix 2 hashes, X and X+1. This function is documented and explains the process of mixing hashes.
PatternSample sample_value_noise_mix_2_hashes(int octave,float frequency,vec2 uv,float time) {
	// In order to make value noise, we have to blend the hash values for each block.
	// To do so, we have to calculate the hash values of adjacent blocks.
	// So we create a hash value with shifted coordinates.bool
	
	// For example doing:
	// h = haser(h, coordinates.x + 1.0); 
	// Gets us the hash of the block on step to the right, 
	// so the resulting pattern is shifted one step to the left.
	
	// To be able to blend the hashes for X and X+1 we need to calculate both.
	// As these are floored coordinates, they are associated with the left side of their blocks. 
	// We make two new variables for each coordinate, h0 for X and h1 for X+1.
	// Then we feed the y coordinate to both to get the final hashers.
	
	// This is the inital hasher.
	// We get the sample coordinates:
	vec2 sample_coordinates = frequency * uv;
	// Then the lattice coordinates, which are just the floored coordinates.bool
	// They are the coordinates where the lattice edges between grid blocks cross.
	vec2 lattice_coordinates = floor(sample_coordinates);
	// Finally the fractional coordinates, 
	// which are the sample coordiantes - lattice coordinates.bool
	// The fractional coordinates are used to blend h0 and h1.
	vec2 fractional_coordinates = sample_coordinates - lattice_coordinates;
	Hasher h = hasher(hash_seed + uint(octave));
	
	Hasher h0 = hasher(h, lattice_coordinates.x);
	Hasher h1 = hasher(h, lattice_coordinates.x + 1.0);
	h0 = hasher(h0, lattice_coordinates.y);
	h1 = hasher(h1, lattice_coordinates.y);
	
	// Derive the two final hash values from these, naming them hash0 and hash1.
	uint hash0 = hash(h0);
	uint hash1 = hash(h1);
	
	// The hash values as floats.
	float v0 = hash_to_float(hash0);
	float v1 = hash_to_float(hash1);
	
	PatternSample s = zero_pattern_sample();
	// Blend the values across the block using the mix() function with fractional coordinates as the interpolator.
	s.v = mix(v0, v1, fractional_coordinates.x);
	return s;
}

float animated_hash_to_float(Hasher h, float t) {
	if(animation) {
		float t0 = floor(t);
		float t1 = t0 + 1.0;
		float v0 = hash_to_float(hash(hasher(h, t0)));
		float v1 = hash_to_float(hash(hasher(h, t1)));
		
		return mix(v0, v1, smoothstep(0.0, 1.0, t - t0));
	}
	else
		return hash_to_float(hash(h));
}

// Mix 4 hashes.
PatternSample sample_value_noise(int octave, float frequency, vec2 uv, float time) {
	vec2 sample_coordinates = frequency * uv;
	vec2 lattice_coordinates = floor(sample_coordinates);
	vec2 fractional_coordinates = sample_coordinates - lattice_coordinates;
	Hasher h = hasher(hash_seed + uint(octave));
	
	Hasher h0 = hasher(h, lattice_coordinates.x);
	Hasher h1 = hasher(h, lattice_coordinates.x + 1.0);
	Hasher h00 = hasher(h0, lattice_coordinates.y);
	Hasher h10 = hasher(h1, lattice_coordinates.y);
	
	Hasher h01 = hasher(h0, lattice_coordinates.y + 1.0);
	Hasher h11 = hasher(h1, lattice_coordinates.y + 1.0);
	
	// Derive the two final hash values from these, naming them hash0 and hash1.
	//uint hash00 = hash(h00);
	//uint hash10 = hash(h10);
	//uint hash01 = hash(h01);
	//uint hash11 = hash(h11);
	
	// The hash values as floats.
	//float v00 = hash_to_float(hash00);
	//float v10 = hash_to_float(hash10);
	//float v01 = hash_to_float(hash01);
	//float v11 = hash_to_float(hash11);
	
	float time_stagger = fract(
			(lattice_coordinates.x + lattice_coordinates.y) * 0.5
	);
	float timeA = time + time_stagger;
	float timeB = time + (0.5 - time_stagger);

	
	float v00 = animated_hash_to_float(h00, timeA);
	float v10 = animated_hash_to_float(h10, timeB);
	float v01 = animated_hash_to_float(h01, timeB);
	float v11 = animated_hash_to_float(h11, timeA);
	
	PatternSample s = zero_pattern_sample();
	// Blend the values across the block using the mix() function with fractional coordinates as the interpolator.
	// This mixing is not smooth since we are performing a linear interpolation.
	//s.v = mix(
			//mix(v00, v10, fractional_coordinates.x),
			//mix(v01, v11, fractional_coordinates.x),
			//fractional_coordinates.y
	//);
	
	// Instead we do a smooth interpolation using the smoothstep() function:
	// y = 3t^2 - 2t^3.
	// We pass the interpolator value (fract coords) through the smoothstep function.
	
	vec2 t = smoothstep(0.0, 1.0, fractional_coordinates);
	s.v = mix(
			mix(v00, v10, t.x),
			mix(v01, v11, t.x),
			t.y
	);
	
	return s;
}

#define SAMPLE_PATTERN_FUNCTION sample_value_noise
#include "res://Shader Library/fractal_pattern.gdshaderinc"

// Take a single float value (0 <> 1) and make a color from it.
// We split the pattern into 2 gradients.
// We begin by doubling our value.
// If its greater than 1 we'll turn it into a gradient from black to white by subtracting 1 from the value.
// Else we subtract the value from 1 and get a gradient from white to black.
// This converts the original single 0-1 gradient to a double 1-0-1 gradient.
vec3 colorize(float v) {
	if(colored) {
		v *= 2.0;
		if(v > 1.0)
			return mix(color_mid, color_max, v - 1.0);
		else
			return mix(color_min, color_mid, v);
	}
	else 
		return vec3(v);
}


void fragment() {
	PatternSample pattern_sample = sample_fractal_pattern(
			standard_fractal_pattern_settings(),
			UV - 0.5,
			TIME
	);
	
	ALBEDO = vec3(colorize(pattern_sample.v));
}
