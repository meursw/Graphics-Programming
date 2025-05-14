extends MeshInstance3D
class_name Fractal

@export_range(1, 8) var depth := 4

func _ready():
	self.name = "Fractal " + str(depth)
	if depth <= 1:
		return
	
	var childA = create_child(Vector3.UP, Quaternion.IDENTITY)
	var childB = create_child(Vector3.RIGHT, Quaternion.from_euler(Vector3(deg_to_rad(0.0), deg_to_rad(0.0), deg_to_rad(-90.0))))
	var childC = create_child(Vector3.LEFT, Quaternion.from_euler(Vector3(deg_to_rad(0.0), deg_to_rad(0.0), deg_to_rad(90.0))))
	var childD = create_child(Vector3.FORWARD, Quaternion.from_euler(Vector3(deg_to_rad(90.0), deg_to_rad(0.0), deg_to_rad(0.0))))
	var childE = create_child(Vector3.BACK, Quaternion.from_euler(Vector3(deg_to_rad(-90.0), deg_to_rad(0.0), deg_to_rad(0.0))))

	add_child(childA)
	add_child(childB)
	add_child(childC)
	add_child(childD)
	add_child(childE)
	
func create_child(direction: Vector3, rotation: Quaternion) -> Fractal:
	var child := duplicate() as Fractal
	child.depth = depth - 1
	child.position = 0.75 * direction
	child.quaternion = rotation
	child.scale = Vector3.ONE * 0.5
	
	return child
