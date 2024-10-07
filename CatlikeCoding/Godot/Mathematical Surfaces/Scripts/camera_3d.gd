extends Camera3D

@export var speed := 5.0
@export var lookaround_speed := 0.0035
var camera_rotation := Vector2.ZERO

var rmb_down := false

func _process(delta):
	if Input.is_action_just_pressed("camera_move_mode"):
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
	if Input.is_action_just_released("camera_move_mode"):
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	
	if Input.is_action_pressed("forward"):
		translate(Vector3.FORWARD * speed * delta)
	if Input.is_action_pressed("back"):
		translate(Vector3.BACK * speed * delta)
		
	if Input.is_action_pressed("left"):
		translate(Vector3.LEFT * delta * speed)
	if Input.is_action_pressed("right"):
		translate(Vector3.RIGHT * delta * speed)
		
	if Input.is_action_pressed("up"):
		translate(Vector3.UP * transform.basis * speed * delta)
	if Input.is_action_pressed("down"):
		translate(Vector3.DOWN * transform.basis * speed * delta)

func _input(event):
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_RIGHT:
			rmb_down = event.pressed
			
	if event is InputEventMouseMotion:
		if rmb_down:
			camera_rotation += event.relative * lookaround_speed * -1.0
			transform.basis = Basis()
			rotate_object_local(Vector3(0, 1, 0), camera_rotation.x)
			rotate_object_local(Vector3(1, 0, 0), camera_rotation.y)
