extends Node
class_name ComputeShader

var step: float 
var time: float 
var resolution: float
var transition_progress: float

var rd: RenderingDevice

var shader: RID
var positions_buffer: RID
var uniforms_buffer: RID
var uniform_set0: RID
var uniform_set1: RID
var pipeline: RID

var multi_mesh: MultiMesh
@export var cube_mesh: Mesh

func _ready() -> void:
	rd = RenderingServer.create_local_rendering_device()
	create_shader()
	resolution = get_parent().resolution
	positions_buffer = rd.storage_buffer_create(resolution * resolution * 3 * 4)
	uniforms_buffer = rd.uniform_buffer_create(64) # will update later
	set_uniforms()
	pipeline = rd.compute_pipeline_create(shader)
	
	multi_mesh = MultiMesh.new()
	multi_mesh.mesh = cube_mesh
	multi_mesh.instance_count = int(resolution * resolution)

func update(s: float, t: float, res: float, tp: float) -> void:
	step = s
	time = t
	resolution = res
	transition_progress = tp

	var ubo_data:PackedFloat32Array = [step, time, resolution, transition_progress]
	var ubo_bytes:PackedByteArray = ubo_data.to_byte_array()
	rd.buffer_update(uniforms_buffer, 0, ubo_bytes.size(), ubo_bytes)

	#dispatch()

func dispatch() -> void:
	var compute_list = rd.compute_list_begin()
	rd.compute_list_bind_compute_pipeline(compute_list, pipeline)
	rd.compute_list_bind_uniform_set(compute_list, uniform_set0, 0)
	rd.compute_list_bind_uniform_set(compute_list, uniform_set1, 1)

	# Dispatch workgroups
	var groups: int = ceil(resolution / 8.0)
	rd.compute_list_dispatch(compute_list, groups, groups, 1)

	rd.compute_list_end()
	rd.submit()

func set_uniforms() -> void:
	# Set up the uniform sets for the shader
	var u_positions = RDUniform.new()
	u_positions.uniform_type = RenderingDevice.UNIFORM_TYPE_STORAGE_BUFFER
	u_positions.binding = 0
	u_positions.add_id(positions_buffer)

	var u_uniforms = RDUniform.new()
	u_uniforms.uniform_type = RenderingDevice.UNIFORM_TYPE_UNIFORM_BUFFER
	u_uniforms.binding = 0
	u_uniforms.add_id(uniforms_buffer)

	uniform_set0 = rd.uniform_set_create([u_positions], shader, 0)
	uniform_set1 = rd.uniform_set_create([u_uniforms], shader, 1)

func create_shader() -> void:
	var shader_file = load("res://Shaders/function_library_compute.glsl")
	var shader_spirv: RDShaderSPIRV = shader_file.get_spirv()
	shader = rd.shader_create_from_spirv(shader_spirv)
