#version 150

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform sampler2D tex0;

 // to enable/disable effects (toggled with f key)
uniform bool enableScanlines;
uniform bool inBloodstream;

out vec4 fragColor;

void main() 
{
vec4 pixel;
vec2 pos = uv0;
pixel = texture(tex0,uv0);

// part 1
// scan lines from demo but edited to be diagonal and decrease width going down the screens
/*
if (enableScanlines) {
	float ptimer = timer/4.0;

	float phase = mod(ptimer,0.07);

	float diagonal = pos.x + 1.5 * pos.y + phase;

	float minWidth = 0.01;  // bottom
	float maxWidth = 0.05;  // top
	float barWidth = mix(maxWidth, minWidth, pos.y);

	float p = mod(diagonal, 0.15);

	if (p < barWidth) {
		pixel = vec4(0,0,0,1);
	}	
}
*/

// part 2
// bloodstream effect
if (inBloodstream) {
    // texture lookups, offset changes with relation to sin(timer * 2.0) so it flows back and forth
    vec2 offset = vec2(0.005, 0.0025) * sin(timer * 2.0);
    vec4 flowPixel = texture(tex0, uv0 + offset);

	// pulsing effect (blood colour pulses in and out)
    float pulse = sin(timer * 4.0) * 0.3 + 0.65;
    vec3 bloodTint = vec3(0.8 * pulse, 0.1, 0.1);
    
    // linear interpolation for blood colour
    vec3 finalColor = mix(pixel.rgb, bloodTint, 0.55);
    
    // distortion (double vision type effect, pulsing of objects) created with the texture lookup from before
    finalColor = mix(finalColor, flowPixel.rgb, 0.2);

   	// tunnel vision effect strength calculation, depends on screen position
    vec2 center = uv0 - vec2(0.5, 0.5);
    float distFromCenter = length(center);
    float tunnelStrength = smoothstep(0.0, 0.7, distFromCenter);

	// vingette/tunnel vision, darker around edges
	float vignette = 1.0 - smoothstep(0.0, 0.6, distFromCenter);
	finalColor *= vignette * 0.8 + 0.6;

    pixel = vec4(finalColor, pixel.a);
}

  fragColor = pixel;
}
