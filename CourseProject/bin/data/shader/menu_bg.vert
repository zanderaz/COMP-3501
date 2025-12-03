#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec2 texcoord;

out vec2 vTexCoord;

// standard vertex shader
void main() {
    vTexCoord = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}