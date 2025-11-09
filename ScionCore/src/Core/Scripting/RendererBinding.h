#pragma once
#include <sol/sol.hpp>

namespace SCION_CORE::ECS {
	class Registry;
}

namespace SCION_CORE::Scripting {

	struct RendererBinder
	{
		static void CreateRenderingBinding(sol::state& lua, SCION_CORE::ECS::Registry& registry);
	};
}