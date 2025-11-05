#pragma once
#include <Rendering/Essentials/Vertex.h>

namespace SCION_CORE::ECS {

	struct UVs
	{
		float u{ 0.0f }, v{ 0.0f }, uv_width{ 0.0f }, uv_height{ 0.0f };
	};


	struct SpriteComponent
	{
		float width{ 0.0f }, height{ 0.0f };
		UVs uvs{ 0.0f, 0.0f, 0.0f, 0.0f };

		SCION_RENDERING::Color color{ 255, 255, 255, 255 };
		int start_x{ 0 }, start_y{ 0 }, layer{ 0 };

		std::string texture_name{""};

		void generate_uvs(int textureWidth, int textureHeight)
		{
			uvs.uv_width = width / textureWidth;
			uvs.uv_height = height / textureHeight;

			uvs.u = start_x * uvs.uv_width;
			uvs.v = start_y * uvs.uv_height;
		}
	};
}