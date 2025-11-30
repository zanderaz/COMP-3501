#version 150

// Passed from the vertex shader
in vec2 uv0;

// Passed from outside
uniform float timer;
uniform sampler2D tex0;

// to enable/disable effects (toggled with f key)
uniform bool inBloodstream;
uniform bool speedBoostActive;

out vec4 fragColor;

// noise generator based on uv coords, cheap way to get static effect
float noiseGen(vec2 p) {
    float h = dot(p, vec2(27.1, 133.7));
    return fract(sin(h) * 43758.5453123);
}

void main() {
	
	vec4 pixel;
	vec2 pos = uv0;
	pixel = texture(tex0,uv0);

	// -------------------------- bloodstream effect ---------------------------
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

	// -------------------------- speed boost effect ---------------------------
	if (speedBoostActive) {

		vec3 baseColor = pixel.rgb;

		// time-based pulse to make the effect time based
		float pulse = 0.75 + 0.25 * sin(timer * 2.0);
		float effectStrength = 1.2 * pulse;

		// spatial logic vars, for vignette effect
		vec2 center = vec2(0.5, 0.5);
		vec2 offset = uv0 - center;
		float dist = length(offset); // radial distance
		float vignette = smoothstep(0.1, 0.8, dist); // 0 at center, 1 near edges

		// radial blur logic
		vec2 dir = normalize(offset + 1e-6);
		vec3 blurAccum = vec3(0.0);
		float weightAccum = 0.0;
		const int NUM_TAPS = 10;

		for (int i = 0; i < NUM_TAPS; ++i) {
			float t = float(i) / float(NUM_TAPS - 1);

			// scale blur radius by vignette and effect strength
			float radius = 0.05 * effectStrength * vignette;
			vec2 sampleUV = uv0 + dir * t * radius;
			vec3 sampleCol = texture(tex0, sampleUV).rgb;

			// slightly bias weights toward farther samples
			float w = 0.6 + 0.4 * t;
			blurAccum += sampleCol * w;
			weightAccum += w;
		}

		vec3 radialBlur = blurAccum / max(weightAccum, 0.0001);

		// chromatic aberration logic
		float chromaAmount = 0.02 * effectStrength * vignette;
		vec2 chromaOffset = dir * chromaAmount;

		float r = texture(tex0, uv0 + chromaOffset).r;
		float g = texture(tex0, uv0 + (chromaOffset / 2.0)).g;
		float b = texture(tex0, uv0 - chromaOffset).b;

		vec3 chromaColor = vec3(r, g, b);

		// combine local effects with base
		vec3 localCombined = mix(baseColor, radialBlur, 0.6);
		localCombined = mix(localCombined, chromaColor, 0.5 * vignette);

		// add static via the noise generator function defined at the top, based on time
		float noise = noiseGen(uv0 * (200.0 + 100.0 * sin(timer * 2.0)) + vec2(timer * 5.0, 0.0));

		// center the noise
		float staticAmount = (noise - 0.5) * 0.4 * vignette * effectStrength;
		vec3 finalColor = localCombined + staticAmount;

		// adding a yellow tint to the edges of the final color
		vec3 yellow = vec3(1.0, 1.0, 0.0);
		float edgeMask = smoothstep(0.3, 0.9, dist);
		float yellowStrength = 0.6 * effectStrength;
		finalColor = mix(finalColor, yellow, edgeMask * yellowStrength);

		// finalize
		finalColor = clamp(finalColor, 0.0, 1.0);
		pixel = vec4(finalColor, pixel.a);
		
	}

	fragColor = pixel;
}
