#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float phase;
layout(location = 3) in float id;

out VS_OUT {
    float phase;
    float id;
} vs_out;

uniform float t;
uniform float pSize;
uniform mat4 MVP;

void main() {

    // Animate position using normal and time

    float g = 2; // slow motion

	float speed = 0.05 +  0.02 * fract(id * 0.17); // vary speed with id

    float movementScale = pSize * 2.0; // Adjust multiplier as needed
    speed *= movementScale;

    vec3 n = normalize(normal);

    vec3 zeroposition = vec3(0,0,0);
    float ext = mod(t+3*phase, 3.0);
    
    // use sin/cos to have interesting trajectory
    vec3 traj = vec3 (sin(t + id * 7.5), sin(t * 0.8 + id * 7), cos(t * 1.2 + id * 6));

    // vec3 animatedPos = zeroposition + speed * traj * ext;

    vec3 animatedPos = zeroposition;
    animatedPos += zeroposition + traj * speed * 0.75;
    animatedPos += 1.2*n * (ext * ext) * speed * 5;

    // no gravity for this
    //animatedPos.y += speed*g*ext*ext; // rising with buoyancy; used ballistic trajectory of dist = 1/2 a t ^2, even though it might not fully make sense
    //animatedPos.y += speed*4*g*ext; // start with upwards velocity


    vec4 a_final = MVP*vec4(animatedPos,1.0); // transformed animated position

	vs_out.phase = phase;
	vs_out.id = id; // passing through vertex phase, id to later in the pipeline

	gl_Position = a_final;

}