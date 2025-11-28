OF_GLSL_SHADER_HEADER

in vec3 FragPos;
in vec3 Normal;
in vec2 uv0; 

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor; // not presently used, mix in if you like
uniform vec3 objectColor;
uniform int specularPower;
uniform bool isLight;
uniform vec3 emissionColor;
uniform sampler2D tex0;
uniform bool useTexture;

out vec4 fragColor;

void main()
{


    // Ambient
    float ambientStrength = 0.1;
    float ambient = ambientStrength;

    // Diffuse
    vec3 N = normalize(Normal); // interpolation might give something that isn't unit length
    vec3 L = normalize(lightPos - FragPos);
    // float diffuse = max(dot(N, L), 0.0);
    float diffuse = 0.5*(1+ dot(N, L)); // new nonphysical "ambient-diffuse" light
    // diffuse = 0;

    // Specular
    float specularStrength = 0.5;
    vec3 V = normalize(viewPos - FragPos);

    // not used for blinn-phong but keeping them here anyway
    // vec3 R = normalize(-L + 2.0*N*dot(L,N));
    // float spec = pow(max(dot(V, R), 0.0), 32);

    // blinn-phong
    vec3 H = normalize((L+V)/2);
    float spec = pow(max(dot(N, H), 0.0), specularPower);
    float specular = specularStrength * spec;
	//specular = 0;
    
    vec3 result;
    vec3 baseColor = objectColor;
    if (useTexture) {
        baseColor = texture(tex0, uv0).rgb;
    }

    if (!useTexture) {
        baseColor = objectColor * 0.8; // Darken slightly to see if it's working
    }

    // for light obj only
    if (isLight) {
        result = emissionColor * baseColor;
    }

    // for other objs
    else {
        result = (ambient + diffuse + specular) * baseColor;
    }

    fragColor = vec4(result.rgb, 1.0);

    //fragColor = vec4(1.0, 0.0, 0.0, 1.0);



/*
if (FragPos.x > 0) fragColor = vec4(1,0,0,1);
    fragColor.rgb = vec3(N.x*0.5+0.5, 0.3, N.z*0.5+0.5);
*/
 //   fragColor = vec4(0.7,0.3,0.1,1.0); // test with constant color
}

