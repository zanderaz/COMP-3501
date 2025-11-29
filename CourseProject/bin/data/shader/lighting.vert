OF_GLSL_SHADER_HEADER

// base vertex shader for lighting code

uniform mat4 modelViewProjectionMatrix;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out vec3 FragPos;
out vec3 Normal; 
out vec2 uv0;

void main() {
    // world space (saved but not in use)
    //FragPos = vec3((worldMatrix * position).xyz); // position in world space
    //Normal = normalize((worldMatrix * vec4(normal.xyz,0)).xyz); // normals in world space; note w=0

    // transform from world to view space
    FragPos = vec3((viewMatrix * worldMatrix * position).xyz);
    Normal = normalize((viewMatrix * worldMatrix * vec4(normal.xyz,0)).xyz);
    uv0 = texcoord;
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * position;
}
