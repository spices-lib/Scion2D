#pragma once
#include "Core/ECS/Registry.h"
#include <Rendering/Core/SpriteBatchRenderer.h>

namespace  SCION_CORE::Systems {

	class RenderSystem
	{
	private:

		SCION_CORE::ECS::Registry& m_Registry;
		std::unique_ptr<SCION_RENDERING::SpriteBatchRenderer> m_pBatchRenderer;

	public:

		RenderSystem(SCION_CORE::ECS::Registry& registry);
		~RenderSystem() = default;

		void Update();
	};
}