#pragma once
#include "Texture.h"
#include <memory>

namespace SCION_RENDERING {

	class TextureLoader
	{
	private:
		static bool LoadTexture(const std::string& filePath, GLuint& id, int& width, int& height, bool blended = false);

	public:

		TextureLoader() = default;
		static std::shared_ptr<Texture> Create(Texture::TextureType type, const std::string& texturePath);
		static std::shared_ptr<Texture> Create(Texture::TextureType type, int width, int height);


	};

}