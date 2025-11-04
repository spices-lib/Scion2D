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

}