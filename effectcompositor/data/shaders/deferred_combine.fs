#version 330 core

out vec4 FragColor;

uniform sampler2D gBLUR;
uniform sampler2D gSSAO;
uniform sampler2D gPosition;
uniform sampler2D gNormal;

uniform int useAO;

in vec2 TexCoords;


void main()
{    
	float ao = texture(gSSAO, TexCoords).r;

	float bao = texture(gBLUR, TexCoords).r;

	vec3 pos = texture(gPosition, TexCoords).rgb;
	vec3 normal = texture(gNormal, TexCoords).rgb;

	//if (useAO == 1)
	{
        FragColor = vec4(ao, ao, ao, 1.0);
	}
	//else
	{
		//FragColor = vec4(bao, bao, bao, 1.0);
	}
	

}