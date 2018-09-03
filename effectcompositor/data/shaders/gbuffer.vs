#version 330 core

in vec4 osg_Vertex;
in vec3 osg_Normal;
in vec4 osg_Color;
in vec4 osg_MultiTexCoord0;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 Color;

uniform mat4 osg_ViewMatrix;
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 osg_ProjectionMatrix;
uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat3 osg_NormalMatrix;

void main()
{
    mat4 worldMatrix = osg_ViewMatrixInverse * osg_ModelViewMatrix;

    vec4 worldPos = worldMatrix * osg_Vertex;
    FragPos = worldPos.xyz; 
    TexCoords = osg_MultiTexCoord0.xy;
    
    mat3 normalMatrix = transpose(inverse(mat3(worldMatrix)));
    Normal = normalMatrix * osg_Normal;
	Normal = normalize(Normal);

	Color = osg_Color.rgb;

    gl_Position = osg_ProjectionMatrix * osg_ViewMatrix * worldMatrix * osg_Vertex;
}