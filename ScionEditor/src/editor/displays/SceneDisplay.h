#pragma once
#include <Core/ECS/Registry.h>
#include "IDisplay.h"

namespace SCION_EDITOR {

	class SceneDisplay : public IDisplay
	{
	private:

		SCION_CORE::ECS::Registry& m_Registry;
		bool m_bPlayScene, m_bSceneLoaded;

	private:

		void LoadScene();
		void UnloadScene();
		void RenderScene();

	public:

		SceneDisplay(SCION_CORE::ECS::Registry& registry);
		~SceneDisplay() override = default;

		void Draw() override;
		void Update() override;
	};
}