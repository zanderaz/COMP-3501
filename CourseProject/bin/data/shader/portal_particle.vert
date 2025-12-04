#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float phase;
layout(location = 3) in float id;

out VS_OUT {
    float phase;
    float id;
    float life; 
} vs_out;

uniform float t;
uniform mat4 MVP;

void main() {

    // lifetime handling
    const float LIFETIME = 10.0;
    float age = mod(t + LIFETIME * phase, LIFETIME);
    float life = age / LIFETIME;

    // set position to what it is in the vbo 
    vec3 basePos = position;

    // in-plane (XZ) radial and tangent directions, derived from basePos
    vec2 baseXZ = basePos.xz;
    vec2 radial = normalize(baseXZ + vec2(0.0001)); // avoid zero-length
    vec2 tangent = vec2(-radial.y, radial.x); // rotate by 90 deg

    // swirl angle varies over time, with small cluster-based offset
    float swirlAngle = 3.0 * life + t * 0.7 + phase * 6.2831;

    // swirl amplitude shrinks over lifetime so paths curl inward a bit
    float swirlAmp = 0.35 * (1.0 - life);
    vec2 swirlOffset =
        swirlAmp * (sin(swirlAngle) * radial + cos(swirlAngle) * tangent);

    // bobbing vertical motion
    float bob = 0.25 * sin(2.5 * t + phase * 10.0);

    vec3 pos = basePos;
    pos.xz += swirlOffset;
    pos.y += bob;

    // small drift along normal to avoid strictly planar motion
    float driftStrength = 0.25;
    pos += normal * driftStrength * sin(4.0 * life + t * 0.5);

    // final clip-space position
    vec4 clipPos = MVP * vec4(pos, 1.0);

    vs_out.phase = phase;
    vs_out.id = id;
    vs_out.life = life;

    gl_Position = clipPos;

}