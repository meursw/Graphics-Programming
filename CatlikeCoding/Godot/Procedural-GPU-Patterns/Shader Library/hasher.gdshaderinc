struct Hasher {
	uint bits;
};

Hasher hasher(uint seed) {
	return Hasher(seed + 374761393u);
}

Hasher hasher(Hasher h, float f) {
	// xxHash algorithim
	// Shuffles the bits around after adding the f value.
	h.bits += floatBitsToUint(f) * 3266489917u;
	// Rotate the bits 17 steps to the left, in two steps.
	h.bits = (h.bits << 17u) | (h.bits >> 32u - 17u);
	// Shuffle again.
	h.bits *= 668265263u;
	return h;
}

// Returns the bits of the hash value.
// Before doing so, perform an Avalanche Effect:
// A minuscule change to the input produces a radically different output.
uint hash(Hasher h) {
	h.bits ^= h.bits >> 15u;
	h.bits *= 2246822519u;
	h.bits ^= h.bits >> 13u;
	h.bits *= 3266489917u;
	h.bits ^= h.bits >> 16u;
	return h.bits;
}

float hash_to_float(uint hash) {
	return uintBitsToFloat(hash & 0x007FFFFFu | 0x3F800000u) - 1.0;
}

vec2 hash_to_vec2(uint hash) {
	return uintBitsToFloat(uvec2(
			hash << 7u,
			hash >> 9u
	) & uvec2(0x007FFF80u) | uvec2(0x3F800000u)) - 1.0;
}

vec3 hash_to_vec3(uint hash) {
	return uintBitsToFloat(uvec3(
			(hash << 12u) & 0x007FF000u,
			(hash << 1u) & 0x007FF000u,
			(hash >> 9u) & 0x007FE000u
	) | uvec3(0x3F800000u)) - 1.0;
}