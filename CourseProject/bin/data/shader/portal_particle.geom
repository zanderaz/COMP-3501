#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float pSize;

in VS_OUT {
    float phase;
    float id;
    float life;
} gs_in[];

out GS_OUT {
    vec2 uv;
    float phase;
    float id;
    float life;
} gs_out;

void main() {

    // center of the particle in clip space
    vec4 center = gl_in[0].gl_Position;

    // screen-aligned quad offsets in clip space
    vec2 offsets[4] = vec2[](
        vec2(-pSize, -pSize),
        vec2( pSize, -pSize),
        vec2(-pSize,  pSize),
        vec2( pSize,  pSize)
    );

    // base UVs for the quad (0..1)
    vec2 baseUV[4] = vec2[](
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(0.0, 1.0),
        vec2(1.0, 1.0)
    );

    for (int i = 0; i < 4; ++i) {
        gs_out.uv = baseUV[i];
        gs_out.phase = gs_in[0].phase;
        gs_out.id = gs_in[0].id;
        gs_out.life = gs_in[0].life;

        vec4 offset = vec4(offsets[i], 0.0, 0.0);
        gl_Position = center + offset;
        EmitVertex();
    }

    EndPrimitive();
}
