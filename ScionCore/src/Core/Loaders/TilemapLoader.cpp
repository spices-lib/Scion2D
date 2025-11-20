#include "TilemapLoader.h"
#include "Core/ECS/Registry.h"
#include "Core/ECS/Components/ComponentSerializer.h"
#include "Core/ECS/Entity.h"
#include <Serializers/JSONSerializer.h>
#include <Logger.h>
#include <error/en.h>
#include <filesystem>

using namespace SCION_FILESYSTEM;
using namespace SCION_CORE::ECS;

namespace fs = std::filesystem;

namespace SCION_CORE::Loaders {

	bool TilemapLoader::SaveTilemapJSON(SCION_CORE::ECS::Registry& registry, const std::string& sTilemapFile)
	{
		std::unique_ptr<JSONSerializer> pSerializer{nullptr};

		try
		{
			pSerializer = std::make_unique<JSONSerializer>(sTilemapFile);
		}
		catch (const std::exception& ex)
		{
			SCION_ERROR("Failed to save tilemap.")
			return false;
		}

		fs::path tilemapPath{ sTilemapFile };
		if (!fs::exists(tilemapPath))
		{
			SCION_ERROR("Filepath does noe exist.");
			return false;
		}

		pSerializer->StartDocument();
		pSerializer->StartNewArray("tilemap");

		auto tiles = registry.GetRegistry().view<TileComponent>();

		for (auto& tile : tiles)
		{
			pSerializer->StartNewObject();
			pSerializer->StartNewObject("components");
			auto tileEnt{ Entity{registry, tile} };

			const auto& transform = tileEnt.GetComponent<TransformComponent>();
			SCION_CORE::ECS::ComponentSerializer::Serialize(*pSerializer, transform);

			const auto& sprite = tileEnt.GetComponent<SpriteComponent>();
			SCION_CORE::ECS::ComponentSerializer::Serialize(*pSerializer, sprite);

			if (tileEnt.HasComponent<BoxColliderComponent>())
			{
				const auto& boxColloder = tileEnt.GetComponent<BoxColliderComponent>();
				SCION_CORE::ECS::ComponentSerializer::Serialize(*pSerializer, boxColloder);
			}

			if (tileEnt.HasComponent<CircleColliderComponent>())
			{
				const auto& circleCollider = tileEnt.GetComponent<CircleColliderComponent>();
				SCION_CORE::ECS::ComponentSerializer::Serialize(*pSerializer, circleCollider);
			}

			if (tileEnt.HasComponent<AnimationComponent>())
			{
				const auto& comp = tileEnt.GetComponent<AnimationComponent>();
				SCION_CORE::ECS::ComponentSerializer::Serialize(*pSerializer, comp);
			}

			if (tileEnt.HasComponent<PhysicsComponent>())
			{
				const auto& comp = tileEnt.GetComponent<PhysicsComponent>();
				SCION_CORE::ECS::ComponentSerializer::Serialize(*pSerializer, comp);
			}

			pSerializer->EndObject();
			pSerializer->EndObject();
		}

		pSerializer->EndArray();

		return pSerializer->EndDocument();
	}

	bool TilemapLoader::LoadTilemapJSON(SCION_CORE::ECS::Registry& registry, const std::string& sTilemapFile)
	{
		std::ifstream mapFile;
		mapFile.open(sTilemapFile);

		if (!mapFile.is_open())
		{
			SCION_ERROR("Failed to open tilemap");
			return false;
		}

		std::stringstream ss;
		ss << mapFile.rdbuf();
		std::string contents = ss.str();
		rapidjson::StringStream jsonStr{ contents.c_str() };

		rapidjson::Document doc;
		doc.ParseStream(jsonStr);

		if (doc.HasParseError() || !doc.IsObject())
		{
			SCION_ERROR("Failed to load tilemap, File is not valid JSON.");
			return false;
		}

		const rapidjson::Value& tilemap = doc["tilemap"];
		if (!tilemap.IsArray() || tilemap.Size() < 1)
		{
			SCION_ERROR("Failed to load tilemap");
			return false;
		}

		for (const auto& tile : tilemap.GetArray())
		{
			Entity newTile{ registry, "", ""};

			const auto& components = tile["components"];

			const auto& jsonTransform = components["transform"];
			auto transform = SCION_CORE::ECS::ComponentSerializer::Deserialize<TransformComponent>(jsonTransform);
			newTile.AddComponent<TransformComponent>(transform);

			const auto& jsonSprite = components["sprite"];
			auto sprite = SCION_CORE::ECS::ComponentSerializer::Deserialize<SpriteComponent>(jsonSprite);
			newTile.AddComponent<SpriteComponent>(sprite);

			if (components.HasMember("boxCollider"))
			{
				const auto& json = components["boxCollider"];
				auto comp = SCION_CORE::ECS::ComponentSerializer::Deserialize<BoxColliderComponent>(json);
				newTile.AddComponent<BoxColliderComponent>(comp);
			}

			if (components.HasMember("circleCollider"))
			{
				const auto& json = components["circleCollider"];
				auto comp = SCION_CORE::ECS::ComponentSerializer::Deserialize<CircleColliderComponent>(json);
				newTile.AddComponent<CircleColliderComponent>(comp);
			}

			if (components.HasMember("animation"))
			{
				const auto& json = components["animation"];
				auto comp = SCION_CORE::ECS::ComponentSerializer::Deserialize<AnimationComponent>(json);
				newTile.AddComponent<AnimationComponent>(comp);
			}

			if (components.HasMember("physics"))
			{
				const auto& json = components["physics"];
				auto comp = SCION_CORE::ECS::ComponentSerializer::Deserialize<PhysicsComponent>(json);
				newTile.AddComponent<PhysicsComponent>(comp);
			}
		}

		mapFile.close();
		return false;
	}

	bool TilemapLoader::SaveTilemap(SCION_CORE::ECS::Registry& registry, const std::string& sTilemapFile, bool bUseJSON)
	{
		if (bUseJSON)
		{
			return SaveTilemapJSON(registry, sTilemapFile);
		}

		return false;
	}

	bool TilemapLoader::LoadTilemap(SCION_CORE::ECS::Registry& registry, const std::string& sTilemapFile, bool bUseJSON)
	{
		if (bUseJSON)
		{
			return LoadTilemapJSON(registry, sTilemapFile);
		}

		return false;
	}

}