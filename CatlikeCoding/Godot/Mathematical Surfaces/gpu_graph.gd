extends Node3D
class_name GPUGraph

enum TransitionMode {Cycle, Random}

@export_range(10, 150, 1) var resolution := 10

@export var transition_mode: TransitionMode
@export var function_name: FunctionLibrary.FunctionName
@export var function_duration := 1.0
@export var transition_duration := 1.0

var duration: float
var transitioning: bool
var transition_function: FunctionLibrary.FunctionName

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
