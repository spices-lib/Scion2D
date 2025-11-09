#include "AssetManager.h"
#include "Rendering/Essentials/ShaderLoader.h"
#include "Rendering/Essentials/TextureLoader.h"
#include "Logger.h"

namespace SCION_RESOURCE {

	bool AssetManager::AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt)
	{
		if (m_mapTextures.contains(textureName))
		{
			SCION_ERROR("Failed to add Texture {0}", textureName);
			return false;
		}

		auto texture = SCION_RENDERING::TextureLoader::Create(
			pixelArt ? SCION_RENDERING::Texture::TextureType::PIXEL : SCION_RENDERING::Texture::TextureType::BLENDED,
			texturePath
		);

		if (!texture)
		{
			SCION_ERROR("Failed to load Texture {0}", textureName);
			return false;
		}

		m_mapTextures[textureName] = texture;
		return true;
	}

	const SCION_RENDERING::Texture& AssetManager::GetTexture(const std::string& textureName)
	{
		if (m_mapTextures.contains(textureName))
		{
			return *m_mapTextures.at(textureName);
		}

		SCION_ERROR("Failed to get Texture {0}", textureName);
		return SCION_RENDERING::Texture();
	}

	bool AssetManager::AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
	{
		if (m_mapShaders.contains(shaderName))
		{
			SCION_ERROR("Failed to add Shader {0}", shaderName);
			return false;
		}

		auto shader = SCION_RENDERING::ShaderLoader::Create(vertexPath, fragmentPath);

		if (!shader)
		{
			SCION_ERROR("Failed to load Shader {0}", shaderName);
			return false;
		}

		m_mapShaders[shaderName] = shader;
		return true;
	}

	SCION_RENDERING::Shader& AssetManager::GetShader(const std::string& shaderName)
	{
		if (m_mapShaders.contains(shaderName))
		{
			return *m_mapShaders.at(shaderName);
		}

		SCION_ERROR("Failed to get Shader {0}", shaderName);

		SCION_RENDERING::Shader s;
		return s;
	}

	bool AssetManager::AddMusic(const std::string& musicName, const std::string& filepath)
	{
		if (!m_mapMusic.contains(musicName))
		{
			SCION_ERROR("Failed to add musci {0}", musicName);
			return false;
		}

		Mix_Music* music = Mix_LoadMUS(filepath.c_str());

		if (!music)
		{
			std::string error{ Mix_GetError() };
			SCION_ERROR("Failed to load : {0}", filepath);
			return false;
		}

		SCION_SOUNDS::SoundParams params {
			.name = musicName,
			.filename = filepath,
			.duration = Mix_MusicDuration(music)
		};

		auto musicPtr = std::make_shared<SCION_SOUNDS::Music>(params, MusicPtr{ music });
		if (!musicPtr)
		{
			SCION_ERROR("Failed to load musci {0}", musicName);
			return false;
		}

		m_mapMusic[musicName] = musicPtr;

		return true;
	}

	std::shared_ptr<SCION_SOUNDS::Music> AssetManager::GetMusic(const std::string& musciName)
	{
		if (!m_mapMusic.contains(musciName))
		{
			SCION_ERROR("Failed to find musci {0}", musciName);
			return nullptr;
		}

		return m_mapMusic.at(musciName);
	}

	bool AssetManager::AddSoundFx(const std::string& soundFxName, const std::string& filepath)
	{
		if (!m_mapSoundFx.contains(soundFxName))
		{
			SCION_ERROR("Failed to add musci {0}", soundFxName);
			return false;
		}

		Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str());

		if (!chunk)
		{
			std::string error{ Mix_GetError() };
			SCION_ERROR("Failed to load : {0}", filepath);
			return false;
		}

		SCION_SOUNDS::SoundParams params {
			.name = soundFxName,
			.filename = filepath,
			.duration = chunk->alen / 179.4
		};

		auto pSoundFx = std::make_shared<SCION_SOUNDS::SoundFX>(params, SoundFxPtr{ chunk });
		if (!pSoundFx)
		{
			SCION_ERROR("Failed to load soundFx {0}", soundFxName);
			return false;
		}

		m_mapSoundFx[soundFxName] = pSoundFx;

		return true;
	}

	std::shared_ptr<SCION_SOUNDS::SoundFX> AssetManager::GetSoundFx(const std::string& soundFxName)
	{
		if (!m_mapSoundFx.contains(soundFxName))
		{
			SCION_ERROR("Failed to find soundFx {0}", soundFxName);
			return nullptr;
		}

		return m_mapSoundFx.at(soundFxName);
	}

	void AssetManager::CreateLuaAssetManager(sol::state& lua, SCION_CORE::ECS::Registry& registry)
	{
		auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();

		lua.new_usertype<AssetManager>(
			"AssetManager",
			sol::no_constructor,
			"add_texture", [&](const std::string& assetName, const std::string& filePath, bool pixel_art) {
				return assetManager->AddTexture(assetName, filePath, pixel_art);
			},
			"add_music", [&](const std::string& musicName, const std::string& filepath){
				return assetManager->AddMusic(musicName, filepath);
			},
			"add_soundFx", [&](const std::string& soundFxName, const std::string& filepath) {
				return assetManager->AddSoundFx(soundFxName, filepath);
			}
		);
	}

}