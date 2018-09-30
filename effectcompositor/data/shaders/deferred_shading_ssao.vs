#version 330 core

in vec4 osg_Vertex;
in vec4 osg_MultiTexCoord0;

out vec2 TexCoords;

uniform mat4 osg_ModelViewProjectionMatrix;

void main()
{
    TexCoords   = osg_MultiTexCoord0.xy;
    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
}