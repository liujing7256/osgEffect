#version 330 core

in vec4 osg_Vertex;
uniform mat4 osg_ModelViewProjectionMatrix;

void main()
{
	gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
}