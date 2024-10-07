extends Node3D

class_name Graph

@export var point_scn: PackedScene
@export_range(10, 100, 1) var resolution := 10
# TO-DO: 
#  add setter that deletes the points 
#  and creates new ones based on the new resoluton

@export var function_name: FunctionLibrary.FunctionName

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
	var f: Callable = FunctionLibrary.get_function(function_name)
	var time := Time.get_ticks_msec() * 0.0005
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
