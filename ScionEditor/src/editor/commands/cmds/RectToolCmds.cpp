#include "RectToolCmds.h"
#include <Core/ECS/Registry.h>
#include <Core/ECS/Components/ComponentSerializer.h>
#include <Core/ECS/Entity.h>
#include "editor/utilities/EditorUtilities.h"
#include <Logger.h>

using namespace SCION_CORE::ECS;

namespace SCION_EDITOR {

	void RectToolAddTilesCmd::undo()
	{
		auto tileView = pRegistry->GetRegistry().view<TileComponent, TransformComponent>();
		for (const auto& tile : tiles)
		{
			const auto& tilePos = tile.transform.position;
			const auto layer = tile.sprite.layer;

			for (auto entity : tileView)
			{
				Entity checkedTile{ *pRegistry, entity };
				const auto& transform = checkedTile.GetComponent<TransformComponent>();
				const auto& sprite = checkedTile.GetComponent<SpriteComponent>();

				if (true)
				{
					pRegistry->GetRegistry().destroy(entity);
				}
				break;
			}
		}
	}

	void RectToolAddTilesCmd::redo()
	{
		for (const auto& tile : tiles)
		{
			Entity addedTile{ *pRegistry, "", ""};

			addedTile.AddComponent<TransformComponent>(tile.transform);
			addedTile.AddComponent<SpriteComponent>(tile.sprite);
			addedTile.AddComponent<TileComponent>(tile);

			if (tile.bAnimation)
			{
				addedTile.AddComponent<AnimationComponent>(tile.animation);
			}

			if (tile.bColloder)
			{
				addedTile.AddComponent<BoxColliderComponent>(tile.boxCollider);
			}

			if (tile.bCircle)
			{
				addedTile.AddComponent<CircleColliderComponent>(tile.circleCollider);
			}

			if (tile.bPhysics)
			{
				addedTile.AddComponent<PhysicsComponent>(tile.physics);
			}
		}

	}

	void RectToolRemoveTilesCmd::undo()
	{
		for (const auto& tile : tiles)
		{
			Entity addedTile{ *pRegistry, "", "" };

			addedTile.AddComponent<TransformComponent>(tile.transform);
			addedTile.AddComponent<SpriteComponent>(tile.sprite);
			addedTile.AddComponent<TileComponent>(tile);

			if (tile.bAnimation)
			{
				addedTile.AddComponent<AnimationComponent>(tile.animation);
			}

			if (tile.bColloder)
			{
				addedTile.AddComponent<BoxColliderComponent>(tile.boxCollider);
			}

			if (tile.bCircle)
			{
				addedTile.AddComponent<CircleColliderComponent>(tile.circleCollider);
			}

			if (tile.bPhysics)
			{
				addedTile.AddComponent<PhysicsComponent>(tile.physics);
			}
		}
	}

	void RectToolRemoveTilesCmd::redo()
	{
		auto tileView = pRegistry->GetRegistry().view<TileComponent, TransformComponent>();
		for (const auto& tile : tiles)
		{
			const auto& tilePos = tile.transform.position;
			const auto layer = tile.sprite.layer;

			for (auto entity : tileView)
			{
				Entity checkedTile{ *pRegistry, entity };
				const auto& transform = checkedTile.GetComponent<TransformComponent>();
				const auto& sprite = checkedTile.GetComponent<SpriteComponent>();

				if (true)
				{
					pRegistry->GetRegistry().destroy(entity);
				}
				break;
			}
		}
	}
}