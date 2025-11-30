#version 150

uniform sampler2D skyTexture;
in vec2 uv0;
in vec3 vDirection;

out vec4 fragColor;

void main() {
    //fragColor = texture(skyTexture, uv0);
    
    vec3 dir = normalize(vDirection);

    float u = 0.5 + atan(dir.z, dir.x) / (2.0 * 3.14159265);
    float v = 0.5 - asin(dir.y) / 3.14159265;

    vec4 colour = texture(skyTexture, vec2(u, v));
    fragColor = colour * 0.9 + 0.2;
}