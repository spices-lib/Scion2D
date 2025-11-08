#pragma once
#include <sol/sol.hpp>
#include <glm/glm.hpp>

namespace SCION_CORE::ECS {

	struct BoxColliderComponent
	{
		int width{ 0 }, height{ 0 };
		glm::vec2 offset{glm::vec2{0}};
		bool bColloding{ false };

		static void CreateLuaBoxColloderBind(sol::state& lua);
	};
}