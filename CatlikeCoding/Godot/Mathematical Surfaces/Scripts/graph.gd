extends Node3D

class_name Graph

enum TransitionMode {Cycle, Random}

@export var point_scn: PackedScene
@export_range(10, 100, 1) var resolution := 10

@export var transition_mode: TransitionMode
@export var function_name: FunctionLibrary.FunctionName
@export var function_duration := 1.0
@export var transition_duration := 1.0

var duration: float
var transitioning: bool
var transition_function: FunctionLibrary.FunctionName

var points: Array[MeshInstance3D]

func _ready():
	var scale := Vector3.ONE * (2.0 / resolution)
	points.resize(resolution * resolution)
	for i in range(0, points.size(), 1):
		var point: MeshInstance3D = point_scn.instantiate()
		points[i] = point
		point.scale = scale
		add_child(point)

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
	
	if(transitioning):
		update_function_transition()
	else:
		update_function()

func update_function() -> void:
	var f: Callable = FunctionLibrary.get_function(function_name)
	var time := Time.get_ticks_msec() * 0.0008
	var step: float = 2.0 / resolution
	var x := 0
	var z := 0
	
	var v := 0.5 * step - 1.0
	for i in range(0, points.size(), 1):
		if(x == resolution):
			x = 0
			z += 1
			v = (z + 0.5) * step - 1.0
		
		var u: float = (x + 0.5) * step - 1.0
		points[i].position = f.call(u, v, time)
		
		x += 1

func update_function_transition() -> void:
	var from: Callable = \
		FunctionLibrary.get_function(transition_function)
	var to: Callable = \
		FunctionLibrary.get_function(function_name)
	var progress: float = duration / transition_duration
	var time := Time.get_ticks_msec() * 0.0008
	var step: float = 2.0 / resolution
	var x := 0
	var z := 0
	
	var v := 0.5 * step - 1.0
	for i in range(0, points.size(), 1):
		if(x == resolution):
			x = 0
			z += 1
			v = (z + 0.5) * step - 1.0
		
		var u: float = (x + 0.5) * step - 1.0
		points[i].position = FunctionLibrary.Morph(u, v, time, from, to, progress)
		
		x += 1

func pick_next_function() -> void:
	function_name = FunctionLibrary.get_next_function(function_name) \
	if transition_mode == TransitionMode.Cycle \
	else FunctionLibrary.get_random_func_other_than(function_name)
