#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace SCION_RENDERING {

	struct Color
	{
		GLubyte r, g, b, a;
	};

	struct Vertex
	{
		glm::vec2 position{ 0.0f }, uvs{ 0.0f };
		Color color{ 255, 255, 255, 255 };

		void set_color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void set_color(GLuint newColor)
		{
			color.r = (newColor >> 24) & 0xFF;
			color.g = (newColor >> 16) & 0xFF;
			color.b = (newColor >> 8) & 0xFF;
			color.a = (newColor >> 0) & 0xFF;
		}
	};

	struct CircleVertex
	{
		glm::vec2 position, uvs;
		Color color;
		float lineThickness;
	};

}