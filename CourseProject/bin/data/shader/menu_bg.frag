#version 150

uniform sampler2D tex0;
uniform float u_time;

in vec2 vTexCoord;
out vec4 outputColor;

void main() {

    vec2 uv = vTexCoord;
    
    // wavy distortion
    uv.x += sin(uv.y * 10.0 + u_time) * 0.01;
    uv.y += cos(uv.x * 10.0 + u_time) * 0.01;
    vec4 texColor = texture(tex0, uv);
    
    // red pulse
    texColor.r *= 1.3 + 0.2 * sin(u_time);
    
    outputColor = texColor;

}
