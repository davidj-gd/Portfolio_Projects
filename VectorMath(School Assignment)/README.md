
### C++ library operations implemented:

EXPORT Vec3 VectorAdd(Vec3 a, Vec3 b);  
EXPORT Vec3 VectorSubtract(Vec3 a, Vec3 b);  
EXPORT Vec3 VectorScale(Vec3 v, float scale);  
EXPORT Vec3 VectorDivide(Vec3 v, float divisor);  
EXPORT float VectorMagnitude(Vec3 v);  
EXPORT float VectorMagnitudeSqr(Vec3 v);  
EXPORT float VectorDistance(Vec3 a, Vec3 b);  
EXPORT Vec3 VectorNormalize(Vec3 v);  
EXPORT float VectorDot(Vec3 a, Vec3 b);  
EXPORT Vec3 VectorCross(Vec3 a, Vec3 b);  
EXPORT Vec3 VectorReflect(Vec3 v, Vec3 normal);  
EXPORT Vec3 VectorLerp(Vec3 a, Vec3 b, float t);  
EXPORT float VectorAngleBetween(Vec3 a, Vec3 b);  
  
EXPORT Vec2 Vector2Add(Vec2 a, Vec2 b);  
EXPORT Vec2 Vector2Subtract(Vec2 a, Vec2 b);  
EXPORT Vec2 Vector2Scale(Vec2 v, float scale);  
EXPORT Vec2 Vector2Divide(Vec2 v, float divisor);  
EXPORT float Vector2Magnitude(Vec2 v);  
EXPORT float Vector2MagnitudeSqr(Vec2 v);  
EXPORT float Vector2Distance(Vec2 a, Vec2 b);  
EXPORT Vec2 Vector2Normalize(Vec2 v);  
EXPORT float Vector2Dot(Vec2 a, Vec2 b);  
EXPORT Vec2 Vector2Reflect(Vec2 v, Vec2 normal);  
EXPORT Vec2 Vector2Lerp(Vec2 a, Vec2 b, float t);  
EXPORT float Vector2AngleBetween(Vec2 a, Vec2 b);  

---------------------------------------------------------------------------------------

### Why vectors and matrices matter for game development

Almost everything that moves or has a position in a game is a vector. The ball position, the velocity, the paddle movement of that is just x, y or x, y, z. Add and scale give you movement, normalize gives you direction without length, reflect is what makes the ball bounce off the paddle instead of going through it. When i understood it that way, i stopped thinking “move the ball” and started thinking “add velocity to position” and “reflect velocity off the normal.” Matrices come in when you need transforms—rotations, camera view, scaling objects,so the same ideas scale up to 3D and more complex games. For Pong i only needed Vec2, but the same operations are the foundation everywhere.

---------------------------------------------------------------------------------------

### Reflection of the implementations made

i implemented a small C++ library with Vec2 and Vec3 operations (add, subtract, scale, normalize, dot, cross, reflect, lerp, etc.), exported it asa single DLL with extern "C" so C# can call it via DllImport. Then in the Pong clone i used that API for the ball and paddles: position and velocity as Vec2, movement as scale + add, and reflect for bounces. The paddles use the same add/scale pattern for moving up and down. So the implementations are the C++ functions themselves, the C# wrapper that loads the DLL, and the game code that actually uses them in BallMovement and PlayerMovement.

---------------------------------------------------------------------------------------

### Deeper implementation analysis & Reflection on learning

The main takeaway for me was separating direction from speed. Velocity is which way (normalize) times how fast (scale with ballSpeed). That way the ball always travels at the same speed but can go any direction, and after a bounce we only change the direction (reflect) and then put the speed back (normalize + scale). Doing it in FixedUpdate with fixedDeltaTime keeps movement consistent regardless of frame rate. you use the dot product with the normal to get how much of the velocity goes “into” the surface, then subtract twice that from the velocity so it bounces. Seeing that in the actual ball behavior tied the math to something concrete. The DLL boundary was a bit of a step, making sure the C struct layout matches C# and that everything is extern "C" but once that worked, calling into C++ from Unity felt straightforward and the rest was just using the same operations everywhere (add for position updates, scale for scaling direction by speed or time).

---------------------------------------------------------------------------------------

### Operations used in Pong:

Vector2Add
Vector2Scale
Vector2Normalize
Vector2Reflect

#### Formula for the used operations:

Vector2Add(a, b)         ->      (a.x + b.x, a.y + b.y)  
Vector2Scale(v, scale)   ->      (v.x * scale, v.y * scale)  
Vector2Normalize(v)      ->      (v.x / length, v.y / length)  
Vector2Reflect(v, n)     ->      dot = v.x * n.x + v.y * n.y  
                                 result = (v.x - 2 * dot * n.x, v.y - 2 * dot * n.y)

---------------------------------------------------------------------------------------

### How i implemented my functions in Pong:

### Ball:
- Used operations: Vector2Add, Vector2Scale, Vector2Normalize, Vector2Reflect
- Movement: I start with declaring the move as Vec2Scale and position as Vec2Add in Fixed Update
```
Vec2 move = VectorMath.Vector2Scale(velocity, Time.fixedDeltaTime);
position = VectorMath.Vector2Add(position, move);
```

- I then randomize which direction the ball should go, left or right (50/50)  
- Vector2Normalize(velocity) makes the length 1 so it just becomes a direction  
- Vector2Scale(velocity, ballSpeed) multiplies the direction by ballSpeed
```
velocity = VectorMath.Vector2Normalize(velocity);
velocity = VectorMath.Vector2Scale(velocity, ballSpeed);
```
- When ball collides with paddle, i get the collision normal and convert Unitys Vector2 to my Vec2
```
Vector2 norm = collision.contacts[0].normal;

Vec2 normal;
normal.x = norm.x;
normal.y = norm.y;
```
- I then reflect the velocity off surface with, so the ball bounces accurately
- then normalize velocity and scale back to my ballSpeed
```
velocity = VectorMath.Vector2Reflect(velocity, normal);
velocity = VectorMath.Vector2Normalize(velocity);
velocity = VectorMath.Vector2Scale(velocity, ballSpeed);
```

### Paddles:
- i declare **private Vec2 position;  
private Vec2 direction;  
private Vec2 movement;**  
- then i use HandleMove() with my Vec2 direction like this
```
direction.x = 0f;
direction.y = input;
```

---------------------------------------------------------------------------------------

