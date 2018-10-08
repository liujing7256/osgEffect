#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;

in vec2 TexCoords;
in vec3 CsPos;
in vec3 CsNormal;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = CsPos;
    // also store the per-fragment normals into the gbuffer
    gNormal   = CsNormal;
}