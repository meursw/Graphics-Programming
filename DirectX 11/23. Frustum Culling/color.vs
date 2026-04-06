
cbuffer MatrixBuffer {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// We make our own type definitions
struct VertexInputType {
	float4 position: POSITION;
	float4 color: COLOR;
};

struct PixelInputType {
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

// The vertex shader is called by the GPU when its processing data from the vertex buffers.
// Returns a pixel input type because it sends the output to the pixel shader.
PixelInputType ColorVertexShader(VertexInputType input) {
	PixelInputType output;
	
	// Change the position vector to be 4 units for proper matrix calculations.
	// Since we only read in a XYZ vector for position.
	input.position.w = 1.0f;
	
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.color = input.color;
	
	return output;
	
}
