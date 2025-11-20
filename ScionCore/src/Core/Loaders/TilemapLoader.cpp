#include "TilemapLoader.h"
#include "Core/ECS/Registry.h"

namespace SCION_CORE::Loaders {

	bool TilemapLoader::SaveTilemapJSON(SCION_CORE::ECS::Registry& registry, const std::string& sTilemapFile)
	{
		return false;
	}

	bool TilemapLoader::LoadTilemapJSON(SCION_CORE::ECS::Registry& registry, const std::string& sTilemapFile)
	{
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