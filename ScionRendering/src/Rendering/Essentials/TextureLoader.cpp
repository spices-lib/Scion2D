#include "TextureLoader.h"
#include <stb_image.h>
#include "Logger.h"

namespace SCION_RENDERING {

	bool TextureLoader::LoadTexture(const std::string& filePath, GLuint& id, int& width, int& height, bool blended)
	{
		int channels = 0;

		auto* image = stbi_load(
			filePath.c_str(),
			&width,
			&height,
			&channels,
			4
		);

		if (!image)
		{
			SCION_ERROR("SOLT failed to load image [ {0} ]", filePath);
			return false;
		}

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (!blended)
		{
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		stbi_image_free(image);

		return true;
	}

	std::shared_ptr<Texture> TextureLoader::Create(Texture::TextureType type, const std::string& texturePath)
	{
		GLuint id;
		int width, height;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		switch (type)
		{
		case Texture::TextureType::PIXEL:
			LoadTexture(texturePath, id, width, height, false);
			break;
		case Texture::TextureType::BLENDED:
			LoadTexture(texturePath, id, width, height, true);
			break;
		default:
			SCION_ERROR("Not supported Texture Type.");
			return nullptr;
		}

		return std::make_shared<Texture>(id, width, height, type, texturePath);
	}

	std::shared_ptr<Texture> TextureLoader::Create(Texture::TextureType type, int width, int height)
	{
		return std::shared_ptr<Texture>();
	}

}