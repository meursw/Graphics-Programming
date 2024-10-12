extends Node3D

class_name Graph

enum TransitionMode {Cycle, Random}

@export var point_scn: PackedScene
@export_range(10, 100, 1) var resolution := 10

@export var transition_mode: TransitionMode
@export var function_name: FunctionLibrary.FunctionName
@export var function_duration := 1.0
@export var transition_duration := 1.0

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

func _on_function_timer_timeout():
	function_name = FunctionLibrary.get_next_function(function_name) \
	if transition_mode == TransitionMode.Cycle \
	else FunctionLibrary.get_random_func_other_than(function_name)
