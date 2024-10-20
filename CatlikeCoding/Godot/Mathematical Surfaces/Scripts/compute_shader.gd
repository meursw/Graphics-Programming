extends Node
class_name ComputeShader

var resolution: float = 0.0

var rd: RenderingDevice
var shader: RID

# Buffers
var positions_buffer: RID
var ubo_buffer: RID
# Uniforms
var positions_uniform: RDUniform
var ubo_uniform: RDUniform

var uniform_set: RID
var uniform_set_textures: RID
var pipeline: RID

# MultiMeshInstance3D node to render using the GPU
@export var mesh_instance3D: MultiMeshInstance3D
@onready var positions: Sprite2D= %Positions

var positions_texture: RID

func _ready():
	rd = RenderingServer.get_rendering_device()
	create_shader()
	pipeline = rd.compute_pipeline_create(shader)
	setup_buffers_and_uniforms()
	setup_positions_texture(resolution, resolution)
	dispatch()

func update(s: float, t: float, res: float, tp: float) -> void:
	update_buffers_and_uniforms(s, t, res, tp)
	dispatch()

func update_buffers_and_uniforms(s: float, t: float, res: float, tp: float) -> void:
	if res != resolution:
			rd.buffer_clear(positions_buffer, 0, resolution * resolution * 3 * 4)
			rd.free_rid(positions_buffer)
			resolution = res
			
			positions_buffer = rd.storage_buffer_create(resolution * resolution * 3 * 4)
			positions_uniform = RDUniform.new()
			positions_uniform.uniform_type = RenderingDevice.UNIFORM_TYPE_STORAGE_BUFFER
			positions_uniform.binding = 0
			positions_uniform.add_id(positions_buffer)
			
			setup_positions_texture(resolution, resolution)
	
	var ubo_data: PackedByteArray = PackedFloat32Array([s, t, resolution, tp]).to_byte_array()
	rd.buffer_update(ubo_buffer, 0, ubo_data.size(), ubo_data)
	
	uniform_set = rd.uniform_set_create([positions_uniform, ubo_uniform], shader, 0)
	
func dispatch() -> void:
	var compute_list := rd.compute_list_begin()
	rd.compute_list_bind_compute_pipeline(compute_list, pipeline)
	rd.compute_list_bind_uniform_set(compute_list, uniform_set, 0)
	rd.compute_list_bind_uniform_set(compute_list, uniform_set_textures, 1)
	var groups: int = ceil(resolution / 8.0)
	rd.compute_list_dispatch(compute_list, groups, groups, 1)
	
	rd.compute_list_end()
	rd.submit()
	rd.sync()  # Ensure the GPU work is done before continuing

func setup_buffers_and_uniforms() -> void:
	resolution = get_parent().resolution
	positions_buffer = rd.storage_buffer_create(resolution * resolution * 3 * 4)
	
	positions_uniform = RDUniform.new()
	positions_uniform.uniform_type = RenderingDevice.UNIFORM_TYPE_STORAGE_BUFFER
	positions_uniform.binding = 0
	positions_uniform.add_id(positions_buffer)
	
	# Create ubo buffer
	var ubo_data: PackedByteArray = PackedFloat32Array([0.0, 0.0, resolution, 0.0]).to_byte_array()
	ubo_buffer = rd.uniform_buffer_create(ubo_data.size(), ubo_data)
	
	ubo_uniform = RDUniform.new()
	ubo_uniform.uniform_type = RenderingDevice.UNIFORM_TYPE_UNIFORM_BUFFER
	ubo_uniform.binding = 1
	ubo_uniform.add_id(ubo_buffer)
	
	uniform_set = rd.uniform_set_create([positions_uniform, ubo_uniform], shader, 0)

func setup_positions_texture(width: int, height: int) -> void:
	# Create texture format
	var tex_format := RDTextureFormat.new()
	tex_format.texture_type = RenderingDevice.TEXTURE_TYPE_2D
	tex_format.width = width
	tex_format.height = height
	tex_format.format = RenderingDevice.DATA_FORMAT_R32G32B32A32_SFLOAT
	tex_format.usage_bits = RenderingDevice.TEXTURE_USAGE_CAN_UPDATE_BIT | RenderingDevice.TEXTURE_USAGE_STORAGE_BIT | RenderingDevice.TEXTURE_USAGE_CAN_COPY_FROM_BIT | RenderingDevice.TEXTURE_USAGE_SAMPLING_BIT

	# Create the texture
	positions_texture = rd.texture_create(tex_format, RDTextureView.new())
	(positions.texture as Texture2DRD).texture_rd_rid = positions_texture

	# Create texture uniform
	var tex_uniform := RDUniform.new()
	tex_uniform.uniform_type = RenderingDevice.UNIFORM_TYPE_IMAGE
	tex_uniform.binding = 0
	tex_uniform.add_id(positions_texture)
	uniform_set_textures = rd.uniform_set_create([tex_uniform], shader, 1)
	
	
func create_shader() -> void:
	shader = rd.shader_create_from_spirv(load("res://Shaders/function_library_compute.glsl").get_spirv())
