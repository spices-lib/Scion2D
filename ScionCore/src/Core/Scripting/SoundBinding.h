#pragma once
#include <sol/sol.hpp>

namespace SCION_CORE::ECS { 
	class Registry;
}

namespace SCION_CORE::Scripting {

	struct SoundBinder
	{
		static void CreateSoundBind(sol::state& lua, SCION_CORE::ECS::Registry& registry);
	};
}