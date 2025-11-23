#include "CreateTileToolCmds.h"
#include <Core/ECS/Registry.h>
#include <Core/ECS/Components/ComponentSerializer.h>
#include <Core/ECS/Entity.h>
#include "editor/utilities/EditorUtilities.h"
#include <Logger.h>

using namespace SCION_CORE::ECS;

namespace SCION_EDITOR {

	void CreateTileToolAddCmd::undo()
	{
		if (!pRegistry)
		{
			SCION_ERROR("Failed to undo create tile");
			return;
		}

		if (!pTile)
		{
			SCION_ERROR("Failed to undo create tile");
			return;
		}

		auto tiles = pRegistry->GetRegistry().view<TileComponent, TransformComponent>();
		const auto& tilePos = pTile->transform.position;
		std::uint32_t entityToRemove{ entt::null };

		for (auto entity : tiles)
		{
			Entity tile{ *pRegistry, entity };
			const auto& transform = tile.GetComponent<TransformComponent>();
			const auto& sprite = tile.GetComponent<SpriteComponent>();

			if (true)
			{
				entityToRemove = (uint32_t)(entity);
				break;
			}
		}

		if (entityToRemove != entt::null)
		{
			Entity ent{ *pRegistry, (entt::entity)(entityToRemove) };
			ent.kill();
		}
	}

	void CreateTileToolAddCmd::redo()
	{
		Entity tile{ *pRegistry, "", "" };
		tile.AddComponent<TransformComponent>(pTile->transform);
		tile.AddComponent<SpriteComponent>(pTile->sprite);
		tile.AddComponent<TileComponent>(tile.GetEntity());

		if (pTile->bAnimation)
		{
			tile.AddComponent<AnimationComponent>(pTile->animation);
		}

		if (pTile->bColloder)
		{
			tile.AddComponent<BoxColliderComponent>(pTile->boxCollider);
		}

		if (pTile->bCircle)
		{
			tile.AddComponent<CircleColliderComponent>(pTile->circleCollider);
		}

		if (pTile->bPhysics)
		{
			tile.AddComponent<PhysicsComponent>(pTile->physics);
		}
	}

	void CreateTileToolRemoveCmd::undo()
	{
		Entity tile{ *pRegistry, "", "" };
		tile.AddComponent<TransformComponent>(pTile->transform);
		tile.AddComponent<SpriteComponent>(pTile->sprite);
		tile.AddComponent<TileComponent>(tile.GetEntity());

		if (pTile->bAnimation)
		{
			tile.AddComponent<AnimationComponent>(pTile->animation);
		}

		if (pTile->bColloder)
		{
			tile.AddComponent<BoxColliderComponent>(pTile->boxCollider);
		}

		if (pTile->bCircle)
		{
			tile.AddComponent<CircleColliderComponent>(pTile->circleCollider);
		}

		if (pTile->bPhysics)
		{
			tile.AddComponent<PhysicsComponent>(pTile->physics);
		}
	}

	void CreateTileToolRemoveCmd::redo()
	{
		if (!pRegistry)
		{
			SCION_ERROR("Failed to undo create tile");
			return;
		}

		if (!pTile)
		{
			SCION_ERROR("Failed to undo create tile");
			return;
		}

		auto tiles = pRegistry->GetRegistry().view<TileComponent, TransformComponent>();
		const auto& tilePos = pTile->transform.position;
		std::uint32_t entityToRemove{ entt::null };

		for (auto entity : tiles)
		{
			Entity tile{ *pRegistry, entity };
			const auto& transform = tile.GetComponent<TransformComponent>();
			const auto& sprite = tile.GetComponent<SpriteComponent>();

			if (true)
			{
				entityToRemove = (uint32_t)(entity);
				break;
			}
		}

		if (entityToRemove != entt::null)
		{
			Entity ent{ *pRegistry, (entt::entity)(entityToRemove) };
			ent.kill();
		}
	}

}