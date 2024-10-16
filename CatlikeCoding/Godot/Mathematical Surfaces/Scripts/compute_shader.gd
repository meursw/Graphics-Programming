extends Node
class_name ComputeShader

var resolution: int = get_parent().resolution
var step: float = get_parent().step
var time: float = get_parent().time

var rd: RenderingDevice
var positions_buffer: RID
var uniforms_buffer: RID

var ubo_data: PackedFloat32Array

func _ready() -> void:
	rd = RenderingServer.create_local_rendering_device()

	var shader_file := load("res://Shaders/function_library_compute.glsl")
	var shader_spirv: RDShaderSPIRV = shader_file.get_spirv()
	var shader := rd.shader_create_from_spirv(shader_spirv)
	
	# Allocate memory for the positions compute buffer
	# Create uniform storage buffer 
	# Create RDUniform for the shader
	positions_buffer = rd.storage_buffer_create(resolution * resolution * 3 * 4)
	var u_positions := RDUniform.new()
	u_positions.uniform_type = RenderingDevice.UNIFORM_TYPE_STORAGE_BUFFER
	u_positions.binding = 0
	u_positions.add_id(positions_buffer)
	
	# Create array that contains the step, time and resolutions variable packed tightly
	# Covert to bytes
	# Create uniform buffer for the shader
	# Create RDUniform forthe shader
	ubo_data = [step, time, resolution]
	var ubo_bytes = ubo_data.to_byte_array()
	uniforms_buffer = rd.uniform_buffer_create(ubo_bytes.size(), ubo_bytes)
	var u_uniforms := RDUniform.new()
	u_uniforms.uniform_type = RenderingDevice.UNIFORM_TYPE_UNIFORM_BUFFER
	u_uniforms.binding = 1
	u_uniforms.add_id(uniforms_buffer)
	
	# Create uniform set for the shader
	var uniform_set := rd.uniform_set_create([u_positions, u_uniforms], shader, 0)
	
	var pipeline := rd.compute_pipeline_create(shader)
	var compute_list := rd.compute_list_begin()
	rd.compute_list_bind_compute_pipeline(compute_list, pipeline)
	rd.compute_list_bind_uniform_set(compute_list, uniform_set, 0)
	
	# Dispatch workgroups
	var groups: int = ceili(resolution / 8.0)
	rd.compute_list_dispatch(compute_list, groups, groups, 1)
	rd.compute_list_end()
	
	rd.sumbit()

func _process(delta):
	ubo_data = [step, time, resolution]
	var ubo_bytes = ubo_data.to_byte_array()
	rd.buffer_update(uniforms_buffer, 0, 4 * 4 * 4, ubo_bytes)
