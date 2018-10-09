#version 330 core

in vec4 osg_Vertex;
in vec3 osg_Normal;
in vec4 osg_Color;
in vec4 osg_MultiTexCoord0;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 WorldNormal;
out vec3 CsPos;
out vec3 CsNormal;
out vec3 Color;

uniform mat4 osg_ViewMatrix;
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 osg_ProjectionMatrix;
uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat3 osg_NormalMatrix;

void main()
{
	TexCoords = osg_MultiTexCoord0.xy;

	mat4 worldMatrix = osg_ViewMatrixInverse * osg_ModelViewMatrix;
	WorldPos = (worldMatrix * osg_Vertex).xyz;

	mat3 normalMatrix = transpose(inverse(mat3(worldMatrix)));
	WorldNormal = normalMatrix * osg_Normal;

	CsPos = (osg_ModelViewMatrix * osg_Vertex).xyz;
	CsNormal = osg_NormalMatrix * osg_Normal;

	Color = osg_Color.rgb;

	gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
}