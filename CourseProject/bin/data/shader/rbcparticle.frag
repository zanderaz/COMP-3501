#version 330
in GS_OUT {
    vec2 uv;
    float phase;
    float id;
} myfrag;

out vec4 outputColor;

uniform float t;
uniform sampler2D texture0;

void main()
{       

    float fade = 1.0-0.3*mod(t+3*myfrag.phase,3.0);

    vec2 uv = myfrag.uv;
/*
// commented out code creates circle
    float d = (uv.x-0.5)*(uv.x-0.5) + (uv.y-0.5)*(uv.y-0.5);

    if (d < 0.2)
       outputColor = vec4(1.0, myfrag.id, 0.2, fade);
    else
       outputColor = vec4(0,0,0,0); // nothing
*/
    
    vec4 col = texture(texture0, uv);

    if (col.r < 0.05 && col.g < 0.05 && col.b < 0.05) {
        discard;
    }

    outputColor = col;
    outputColor.a = fade;
}
