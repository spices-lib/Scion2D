#include "CreateTileTool.h"
#include <Core/Resources/AssetManager.h>
#include <Core/ECS/MainRegistry.h>
#include <Rendering/Core/Camera.h>
#include <Logger.h>
#include "editor/utilities/EditorUtilities.h"

namespace SCION_EDITOR {

	CreateTileTool::CreateTileTool()
		: TileTool()
	{}

	void CreateTileTool::Create()
	{
	}

	void CreateTileTool::Draw()
	{
		if (!SpriteValid() || !IsActivated() || OutOfBounds() || !m_pCamera)
			return;

		auto& pShader = SCION_CORE::ECS::MainRegistry::GetInstance().GetAssetManager().GetShader("basic");

		pShader.Enable();
		auto camMat = m_pCamera->GetCameraMatrix();
		pShader.SetUniformMat4("uProjection", camMat);

		DrawMouseSprite();
		pShader.Disable();
	}

}