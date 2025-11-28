#version 150

uniform sampler2D skyTexture;
in vec2 uv0;
in vec3 vDirection;

out vec4 fragColor;

void main() {
    // For spherical mapping, you can use the sphere's built-in UVs
    // or convert 3D position to spherical coordinates
    fragColor = texture(skyTexture, uv0);

    
    /* if we want rectangular mapping
    vec3 dir = normalize(vDirection);

    float u = 0.5 + atan(dir.z, dir.x) / (2.0 * 3.14159265);
    float v = 0.5 - asin(dir.y) / 3.14159265;

    fragColor = texture(skyTexture, vec2(u, v));
    */
}