OF_GLSL_SHADER_HEADER

uniform mat4 modelViewProjectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out vec2 uv0;
out vec3 vDirection;

void main() {
    uv0 = texcoord;
    
    // Remove camera translation but keep rotation
    mat4 rotOnlyView = viewMatrix;
    rotOnlyView[3][0] = 0.0;
    rotOnlyView[3][1] = 0.0;
    rotOnlyView[3][2] = 0.0;

    gl_Position = projectionMatrix * rotOnlyView * position;
    //gl_Position = projectionMatrix * viewMatrix * position;

    vDirection = (rotOnlyView * position).xyz;

    // Push to far depth
    gl_Position.z = gl_Position.w * 0.9999;
}
