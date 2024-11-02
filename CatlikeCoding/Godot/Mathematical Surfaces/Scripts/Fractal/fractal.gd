extends MeshInstance3D
class_name Fractal

@export_range(1, 8) var depth := 4

func _ready():
	self.name = "Fractal " + str(depth)
	if depth <= 1:
		return
	
	var childA = create_child(Vector3.UP, Quaternion.IDENTITY)
	var childB = create_child(Vector3.RIGHT, Quaternion.from_euler(Vector3(0.0, 0.0, -90.0)))
	var childC = create_child(Vector3.LEFT, Quaternion.from_euler(Vector3(0.0, 0.0, 90.0)))

	add_child(childA)
	add_child(childB)
	add_child(childC)
	
func create_child(direction: Vector3, rotation: Quaternion) -> Fractal:
	var child: Fractal = duplicate()
	child.depth = depth - 1;
	child.position = 0.75 * direction
	child.quaternion = rotation
	child.scale = 0.5 * Vector3.ONE
	
	return child
