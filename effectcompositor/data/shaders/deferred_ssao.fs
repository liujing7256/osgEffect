#version 330 core

out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gNoise;

uniform mat4  projectionMatrix;
uniform mat4  viewMatrix;

uniform float nearPlaneValue;
uniform float farPlaneValue;
uniform float viewportWidth;
uniform float viewportHeight;

vec3 samples[64] = vec3[](
	vec3(0.0497709, -0.0447092, 0.0499634),
	vec3(0.0145746, 0.0165311, 0.00223862),
	vec3(-0.0406477, -0.0193748, 0.0319336),
	vec3(0.0137781, -0.091582, 0.0409242),
	vec3(0.055989, 0.0597915, 0.0576589),
	vec3(0.0922659, 0.0442787, 0.0154511),
	vec3(-0.00203926, -0.054402, 0.066735),
	vec3(-0.00033053, -0.000187337, 0.000369319),
	vec3(0.0500445, -0.0466499, 0.0253849),
	vec3(0.0381279, 0.0314015, 0.032868),
	vec3(-0.0318827, 0.0204588, 0.0225149),
	vec3(0.0557025, -0.0369742, 0.0544923),
	vec3(0.0573717, -0.0225403, 0.0755416),
	vec3(-0.0160901, -0.00376843, 0.0554733),
	vec3(-0.0250329, -0.024829, 0.0249512),
	vec3(-0.0336879, 0.0213913, 0.0254024),
	vec3(-0.0175298, 0.0143856, 0.00534829),
	vec3(0.0733586, 0.112052, 0.0110145),
	vec3(-0.0440559, -0.0902836, 0.083683),
	vec3(-0.0832772, -0.00168341, 0.0849867),
	vec3(-0.0104057, -0.0328669, 0.019273),
	vec3(0.00321131, -0.00488206, 0.00416381),
	vec3(-0.00738321, -0.0658346, 0.067398),
	vec3(0.0941413, -0.00799846, 0.14335),
	vec3(0.0768329, 0.126968, 0.106999),
	vec3(0.000392719, 0.000449695, 0.00030161),
	vec3(-0.104793, 0.0654448, 0.101737),
	vec3(-0.00445152, -0.119638, 0.161901),
	vec3(-0.0745526, 0.0344493, 0.224138),
	vec3(-0.0027583, 0.00307776, 0.00292255),
	vec3(-0.108512, 0.142337, 0.166435),
	vec3(0.046882, 0.103636, 0.0595757),
	vec3(0.134569, -0.0225121, 0.130514),
	vec3(-0.16449, -0.155644, 0.12454),
	vec3(-0.187666, -0.208834, 0.0577699),
	vec3(-0.043722, 0.0869255, 0.0747969),
	vec3(-0.00256364, -0.00200082, 0.00406967),
	vec3(-0.0966957, -0.182259, 0.299487),
	vec3(-0.225767, 0.316061, 0.089156),
	vec3(-0.0275051, 0.287187, 0.317177),
	vec3(0.207216, -0.270839, 0.110132),
	vec3(0.0549017, 0.104345, 0.323106),
	vec3(-0.13086, 0.119294, 0.280219),
	vec3(0.154035, -0.0653706, 0.229842),
	vec3(0.0529379, -0.227866, 0.148478),
	vec3(-0.187305, -0.0402247, 0.0159264),
	vec3(0.141843, 0.0471631, 0.134847),
	vec3(-0.0442676, 0.0556155, 0.0558594),
	vec3(-0.0235835, -0.0809697, 0.21913),
	vec3(-0.142147, 0.198069, 0.00519361),
	vec3(0.158646, 0.230457, 0.0437154),
	vec3(0.03004, 0.381832, 0.163825),
	vec3(0.083006, -0.309661, 0.0674131),
	vec3(0.226953, -0.23535, 0.193673),
	vec3(0.381287, 0.332041, 0.529492),
	vec3(-0.556272, 0.294715, 0.301101),
	vec3(0.42449, 0.00564689, 0.117578),
	vec3(0.3665, 0.00358836, 0.0857023),
	vec3(0.329018, 0.0308981, 0.178504),
	vec3(-0.0829377, 0.512848, 0.0565553),
	vec3(0.867363, -0.00273376, 0.100138),
	vec3(0.455745, -0.772006, 0.0038413),
	vec3(0.417291, -0.154846, 0.462514),
	vec3(-0.442722, -0.679282, 0.186503));

vec3 getCsPosition()
{
	return vec3(texture(gPosition, TexCoords).rgb);

	//vec4 worldPos = vec4(texture(gPosition, TexCoords).rgb, 1.0);
	//return (viewMatrix * worldPos).xyz;
}

vec3 getCsNormal()
{
	return vec3(texture(gNormal, TexCoords).rgb);

	//vec3 worldNormal  = vec3(texture(gNormal, TexCoords).rgb);
	//mat3 normalMatrix = transpose(inverse(mat3(viewMatrix)));
	//return (normalMatrix * worldNormal).xyz;
}

float getSampleDepth(in vec2 st)
{
	return vec3(texture(gPosition, st).rgb).z;

	//vec4 worldPos = vec4(texture(gPosition, TexCoords).rgb, 1.0);
	//return (viewMatrix * worldPos).z;
}

vec3 getRandomVec()
{
	vec2 noiseScale = vec2(viewportWidth / 4.0, viewportHeight / 4.0);
	return normalize(texture(gNoise, TexCoords * noiseScale).xyz);
}

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
float radius = 1000.0;
float bias = 0.5;

void main()
{
	// get input for SSAO algorithm
	vec3 fragPos   = getCsPosition();
	vec3 normal    = getCsNormal();
	vec3 randomVec = getRandomVec();
	// create TBN change-of-basis matrix: from tangent-space to view-space
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	// iterate over the sample kernel and calculate occlusion factor
	float occlusion = 0.0;
	for (int i = 0; i < kernelSize; ++i)
	{
		// get sample position
		vec3 sample = TBN * samples[i]; // from tangent to view-space
		sample = fragPos + sample * radius;

		// project sample position (to sample texture) (to get position on screen/texture)
		vec4 offset = vec4(sample, 1.0);
		offset = projectionMatrix * offset; // from view to clip-space
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

		// get sample depth
		float sampleDepth = getSampleDepth(offset.xy); // get depth value of kernel sample

		// range check & accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
	}
	occlusion = 1.0 - (occlusion / kernelSize);

	FragColor = occlusion;
}
