#pragma once
#include "Core/ECS/Registry.h"

namespace SCION_RENDERING
{
	class Camera2D;
	class SpriteBatchRenderer;
}

namespace  SCION_CORE::Systems {

	class RenderSystem
	{
	private:

		SCION_CORE::ECS::Registry& m_Registry;
		std::unique_ptr<SCION_RENDERING::SpriteBatchRenderer> m_pBatchRenderer;

	public:

		RenderSystem(SCION_CORE::ECS::Registry& registry);
		~RenderSystem() = default;

		void Update(SCION_RENDERING::Camera2D& camera);
	};
}