#pragma once
#include <map>
#include <memory>
#include <string>

#include <Rendering/Essentials/Shader.h>
#include <Rendering/Essentials/Texture.h>
#include <Rendering/Essentials/Font.h>
#include <Sounds/Essentials/Music.h>
#include <Sounds/Essentials/SoundFX.h>
#include <sol/sol.hpp>
#include "Core/ECS/Registry.h"

namespace SCION_RESOURCE {

	class AssetManager 
	{
	private:

		std::map<std::string, std::shared_ptr<SCION_RENDERING::Texture>> m_mapTextures{};
		std::map<std::string, std::shared_ptr<SCION_RENDERING::Shader>> m_mapShaders{};
		std::map<std::string, std::shared_ptr<SCION_RENDERING::Font>> m_mapFonts{};

		std::map<std::string, std::shared_ptr<SCION_SOUNDS::Music>> m_mapMusic{};
		std::map<std::string, std::shared_ptr<SCION_SOUNDS::SoundFX>> m_mapSoundFx{};

	public:

		AssetManager() = default;
		~AssetManager() = default;

		bool CreateDefaultFonts();

		bool AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt = true);
		bool AddTextureFromMemory(const std::string& textureName, const unsigned char* imageData, size_t length, bool pixelArt);
		const SCION_RENDERING::Texture& GetTexture(const std::string& textureName);

		bool AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		SCION_RENDERING::Shader& GetShader(const std::string& shaderName);

		bool AddMusic(const std::string& musicName, const std::string& filepath);
		std::shared_ptr<SCION_SOUNDS::Music> GetMusic(const std::string& musciName);

		bool AddSoundFx(const std::string& soundFxName, const std::string& filepath);
		std::shared_ptr<SCION_SOUNDS::SoundFX> GetSoundFx(const std::string& soundFxName);

		bool AddFont(const std::string& fontName, const std::string& filepath, float fontSize = 32.0f);
		bool AddFontFromMemory(const std::string& fontName, unsigned char* fontData, float fontSize = 32.0f);
		std::shared_ptr<SCION_RENDERING::Font> GetFont(const std::string& fontName);

		static void CreateLuaAssetManager(sol::state& lua, SCION_CORE::ECS::Registry& registry);
	};
}