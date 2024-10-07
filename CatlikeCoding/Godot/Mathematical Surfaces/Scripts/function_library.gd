class_name FunctionLibrary

enum FunctionName {Wave, MultiWave, Ripple, Sphere, Torus}

static var functions: Array[Callable] = [Wave, MultiWave, Ripple, Sphere, Torus]

static func get_function(name: FunctionName) -> Callable:
	return functions[int(name)]

static func Wave(u: float, v: float, t: float) -> Vector3:
	var p: Vector3
	p.x = u
	p.y = sin(PI * (u + v + t))
	p.z = v
	return p

static func MultiWave(u: float, v: float, t: float) -> Vector3:
	var p: Vector3
	p.x = u
	p.y = sin(PI * (u + 0.5 * t))
	p.y += 0.5 * sin(2 * PI * (v + t))
	p.y += sin(PI * (u * v + 0.25 * t))
	p.y *= 1.0 / 2.5
	p.z = v
	return p

static func Ripple(u: float, v: float, t: float) -> Vector3:
	
	var d: float = sqrt(u * u + v * v) # magnitude
	var p: Vector3 
	p.x = u
	p.y = sin(PI * (4.0 * d - t))
	p.y /= 1.0 + 10.0 * d
	p.z = v
	return p

static func Sphere(u: float, v: float, t: float) -> Vector3:
	var p: Vector3
	var radius: float = 0.9 + 0.1 * sin(PI * (6.0 * u + 4.0 * v + t));
	var scale: float = radius * cos(0.5 * PI * v)
	p.x = scale * sin(PI * u)
	p.y = radius * sin(PI * 0.5 * v) 
	p.z = scale * cos(PI * u)
	return p

static func Torus(u: float, v: float, t: float) -> Vector3:
	var r1: float = 0.7 + 0.1 * sin(PI * (6.0 * u + 0.5 * t));
	var r2: float = 0.15 + 0.05 * sin(PI * (8.0 * u + 4.0 * v + 2.0 * t));
	
	var scale: float = r1 + r2 * cos(PI * v)
	
	var p: Vector3
	p.x = scale * sin(PI * u)
	p.y = r2 * sin(PI * v)
	p.z = scale * cos(PI * u)
	return p
