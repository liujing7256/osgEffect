#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 WorldNormal;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = WorldPos;
    // also store the per-fragment normals into the gbuffer
    gNormal   = WorldNormal;
}