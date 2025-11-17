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
}