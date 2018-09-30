#version 330 core

#pragma import_defines ( ALBEDOMAP METALLICMAP ROUGHNESSMAP AOMAP NORMALMAP )

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gRMA;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 WorldNormal;
in vec3 Color;

// material parameters
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform vec3  albedo;

#ifdef ALBEDOMAP
uniform sampler2D albedoMap;
#endif

#ifdef METALLICMAP
uniform sampler2D metallicMap;
#endif

#ifdef ROUGHNESSMAP
uniform sampler2D roughnessMap;
#endif

#ifdef AOMAP
uniform sampler2D aoMap;
#endif

vec3 getAlbedo()
{
#ifdef ALBEDOMAP
	return vec3(pow(texture(albedoMap, TexCoords).rgb, vec3(2.2)));
#else
	return albedo;
#endif
}

float getRoughness()
{
#ifdef ROUGHNESSMAP
	return texture(roughnessMap, TexCoords).r;
#else
	return roughness;
#endif
}

float getMetallic()
{
#ifdef METALLICMAP
	return texture(metallicMap, TexCoords).r;
#else
	return metallic;
#endif
}

float getAo()
{
#ifdef AOMAP
	return texture(aoMap, TexCoords).r;
#else
	return ao;
#endif
}

#ifdef NORMALMAP
uniform sampler2D normalMap;
vec3 getNormalFromMap()
{
	vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

	vec3 Q1 = dFdx(WorldPos);
	vec3 Q2 = dFdy(WorldPos);
	vec2 st1 = dFdx(TexCoords);
	vec2 st2 = dFdy(TexCoords);

	vec3 N = normalize(WorldNormal);
	vec3 T = normalize(Q1*st2.t - Q2 * st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}
#endif
vec3 getWorldNormal()
{
#ifdef NORMALMAP
	return getNormalFromMap();
#else
	return WorldNormal;
#endif
}

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = WorldPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = getWorldNormal();

    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = getAlbedo();
    gAlbedoSpec.a   = 1.0;

	// add roughness, metallic ao parametters.
	gRMA.r = getRoughness();
	gRMA.g = getMetallic();
	gRMA.b = getAo();
}