#version 330
in GS_OUT {
    vec2 uv;
    float phase;
    float id;
} myfrag;

out vec4 outputColor;

uniform float t;
uniform sampler2D texture0;

void main() {       

    float fade = 1.0 - 0.3 * mod(t + 3 * myfrag.phase, 3.0);

    vec2 uv = myfrag.uv;
    
    vec4 col = texture(texture0, uv);

    if (col.r < 0.05 && col.g < 0.05 && col.b < 0.05) {
        discard;
    }

    outputColor = col;
    outputColor.a = fade;
}
