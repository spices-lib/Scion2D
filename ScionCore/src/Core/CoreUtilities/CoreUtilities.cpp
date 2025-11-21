#include "CoreUtilities.h"

namespace SCION_CORE {

	bool EntityInView(const ECS::TransformComponent& transform, float width, float height, const SCION_RENDERING::Camera2D& camera)
	{
		const auto& cameraPos = camera.GetPosition();
		const auto& cameraWidth = camera.GetWidth();
		const auto& cameraHeight = camera.GetHeight();
		const auto& cameraScale = camera.GetScale();

		if ((transform.position.x <= (cameraPos.x - (width * transform.scale.x * cameraScale)) / cameraScale))
		{
			return false;
		}

		return true;
	}

	glm::mat4 RSTModel(const ECS::TransformComponent& transform, float width, float height)
	{
		return glm::mat4(1.0f);
	}

	void GenerateUVs(SCION_CORE::ECS::SpriteComponent& sprite, int textureWidth, int textureHeight)
	{
		sprite.uvs.uv_width = sprite.width / textureWidth;
		sprite.uvs.uv_height = sprite.height / textureHeight;
		sprite.uvs.u = sprite.start_x * sprite.uvs.uv_width;
		sprite.uvs.v = sprite.start_y * sprite.uvs.uv_height;
	}
}