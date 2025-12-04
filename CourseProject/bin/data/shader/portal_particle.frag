#version 330

in GS_OUT {
    vec2 uv;
    float phase;
    float id;
    float life;
} myfrag;

out vec4 outputColor;

uniform float t;
uniform sampler2D texture0;

void main() {
    vec4 texColor = texture(texture0, myfrag.uv);

    // texture is greyscale so this is appropriate
    float intensity = texColor.r;

    // dark -> transparent, bright -> opaque
    float mask = smoothstep(0.02, 0.20, intensity);

    // time-based fade + flicker
    float alphaFade = 1.0 - smoothstep(0.0, 1.0, myfrag.life);
    float flicker = 0.85 + 0.15 * sin(12.0 * t + myfrag.phase * 25.0);

    float finalAlpha = mask * alphaFade * flicker;

    // discard if effectively invisible
    if (finalAlpha <= 0.001) {
        discard;
    }

    // finalize
    vec3 portalColor = texColor.rgb;
    outputColor = vec4(portalColor, finalAlpha);

}
