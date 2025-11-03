#pragma once
#include <glm/glm.hpp>

namespace SCION_CORE::ECS {

	struct TransformComponent
	{
		glm::vec2 position{glm::vec2{0.0f}}, scale{ glm::vec2{1.0f} };
		float rotation{ 0.0f };
	};

}