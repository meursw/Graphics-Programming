extends Node3D
class_name GPUGraph

enum TransitionMode {Cycle, Random}

@export_group("GPU Graph")
@export_range(10, 200, 1) var resolution := 10
@export var function_name: FunctionLibrary.FunctionName
@export var transition_mode: TransitionMode
@export var function_duration := 1.0
@export var transition_duration := 1.0

var duration: float
var transitioning: bool
var transition_function: FunctionLibrary.FunctionName

var step: float = 2.0 / resolution
var time: float = Time.get_ticks_msec() * 0.0008

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

func pick_next_function() -> void:
	function_name = FunctionLibrary.get_next_function(function_name) \
	if transition_mode == TransitionMode.Cycle \
	else FunctionLibrary.get_random_func_other_than(function_name)
