#include "CreateTileTool.h"
#include <Core/Resources/AssetManager.h>
#include <Core/ECS/MainRegistry.h>
#include <Rendering/Core/Camera.h>
#include <Logger.h>
#include "editor/utilities/EditorUtilities.h"
#include <Core/ECS/Entity.h>
#include <Core/ECS/Components/TileComponent.h>
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include "editor/commands/CommandManager.h"

using namespace SCION_CORE::ECS;

namespace SCION_EDITOR {

	void CreateTileTool::CreateNewTile()
	{
		const auto& mouseWorldCoords = GetMouseWorldCoords();

		if (auto id = CheckForTile(mouseWorldCoords); id != entt::null)
			return;

		Entity tile{ CreateEntity() };

		tile.AddComponent<TransformComponent>(m_pMouseTile->transform);
		tile.AddComponent<SpriteComponent>(m_pMouseTile->sprite);

		if (m_pMouseTile->bColloder)
		{

		}

		if (m_pMouseTile->bAnimation)
		{

		}

		if (m_pMouseTile->bPhysics)
		{

		}

		tile.AddComponent<TileComponent>(static_cast<uint32_t>(tile.GetEntity()));

		auto createToolAddCmd = UndoableCommands{
			CreateTileToolAddCmd{
				.pRegistry = SceneManager::GetInstance().GetCurrentScene()->GetRegistryPtr(),
				.pTile = std::make_shared<Tile>(*m_pMouseTile)
			}
		};

		SceneManager::GetInstance().GetCommandManager().Execute(createToolAddCmd);
	}

	void CreateTileTool::RemoveTile()
	{

	}

	CreateTileTool::CreateTileTool()
		: TileTool()
	{}

	void CreateTileTool::Create()
	{
		if (OutOfBounds() || !IsActivated() || !SpriteValid() || !m_pRegistry)
			return;

		if (MouseBtnJustPressed(EMouseButton::LEFT) || MouseBtnPressed(EMouseButton::LEFT))
		{
			CreateNewTile();
		}
		else if (MouseBtnJustPressed(EMouseButton::RIGHT) || MouseBtnPressed(EMouseButton::RIGHT))
		{
			RemoveTile();
		}


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