#pragma once
#include <map>
#include <memory>
#include <string>

#include <Rendering/Essentials/Shader.h>
#include <Rendering/Essentials/Texture.h>
#include <sol/sol.hpp>
#include "Core/ECS/Registry.h"

namespace SCION_RESOURCE {

	class AssetManager 
	{
	private:

		std::map<std::string, std::shared_ptr<SCION_RENDERING::Texture>> m_mapTextures{};
		std::map<std::string, std::shared_ptr<SCION_RENDERING::Shader>> m_mapShaders{};

	public:

		AssetManager() = default;
		~AssetManager() = default;

		bool AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt = true);
		const SCION_RENDERING::Texture& GetTexture(const std::string& textureName);

		bool AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		SCION_RENDERING::Shader& GetShader(const std::string& shaderName);

		static void CreateLuaAssetManager(sol::state& lua, SCION_CORE::ECS::Registry& registry);
	};
}