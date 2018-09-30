#version 330 core

out float FragColor;

in vec2 TexCoords;

uniform sampler2D gSSAO;

uniform float nearPlaneValue;
uniform float farPlaneValue;
uniform float viewportWidth;
uniform float viewportHeight;

void main()
{
	vec2 texelSize = 1.0 / vec2(textureSize(gSSAO, 0));
	float result = 0.0;
	for (int x = -8; x < 8; ++x)
	{
		for (int y = -8; y < 8; ++y)
		{
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			result += texture(gSSAO, TexCoords + offset).r;
		}
	}

	FragColor = result / (64.0 * 64.0);
}
