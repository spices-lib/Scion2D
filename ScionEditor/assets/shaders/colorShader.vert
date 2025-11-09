#version 450

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec4 vertexColor;

out vec4 fragmentColor;

uniform mat4 projection;

void main()
{
	glPosition.xy = (projection * vec4(vertexPosition, 0.0, 1.0)).xy;
	glPosition.z = 0.0;
	glPosition.w = 1.0;

	fragmentColor = vertexColor;
}