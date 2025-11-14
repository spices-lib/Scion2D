#pragma once
#include <Core/ECS/Registry.h>
#include "IDisplay.h"

namespace SCION_EDITOR {

	class SceneDisplay : public IDisplay
	{
	private:

		SCION_CORE::ECS::Registry& m_Registry;

	public:

		SceneDisplay(SCION_CORE::ECS::Registry& registry);
		~SceneDisplay() override = default;

		void Draw() override;
	};
}