#pragma once
#include "Core/ECS/Components/TransformComponent.h"
#include <Rendering/Core/Camera.h>

namespace SCION_CORE {

	bool EntityInView(const ECS::TransformComponent& transform, float width, float height, const SCION_RENDERING::Camera2D& camera);
	glm::mat4 RSTModel(const ECS::TransformComponent& transform, float width, float height);
}