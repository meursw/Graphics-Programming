extends Node3D
class_name GPUGraph

enum TransitionMode {Cycle, Random}

@export_group("GPU Graph")
@export_range(10, 200, 1) var resolution: float = 10
@export var function_name: FunctionLibrary.FunctionName
@export var transition_mode: TransitionMode
@export var function_duration := 1.0
@export var transition_duration := 1.0
@export var compute_shader: Node
@export_group("Procedural Drawing")

var duration: float
var transitioning: bool
var transition_function: FunctionLibrary.FunctionName

var step: float = 2.0 / resolution
var time: float = Time.get_ticks_msec() * 0.0008
var transition_progress: float = 0.0

func _process(delta):
	duration += delta
	if(transitioning):
		if(duration >= transition_duration):
			duration -= transition_duration
			transitioning = false
		
	elif(duration >= function_duration):
		duration -= function_duration
		transitioning = true;
		transition_function = function_name;
		pick_next_function()
	
	update_function_on_gpu()

func update_function_on_gpu() -> void:
	step = 2.0 / resolution
	time = Time.get_ticks_msec() * 0.0008
	transition_progress = duration / transition_duration
	compute_shader.update(step, time, resolution, transition_progress)
	

func pick_next_function() -> void:
	function_name = FunctionLibrary.get_next_function(function_name) \
	if transition_mode == TransitionMode.Cycle \
	else FunctionLibrary.get_random_func_other_than(function_name)
