#version 460 core

in vec2 fragUVs;
in vec4 fragColor;

out vec4 color;

uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture, fragUVs);
}