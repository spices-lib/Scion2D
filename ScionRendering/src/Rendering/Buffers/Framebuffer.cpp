#include "Framebuffer.h"
#include <Logger.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/Texture.h>

namespace SCION_RENDERING {

	bool Framebuffer::Initialize()
	{
		return false;
	}

	Framebuffer::Framebuffer()
		: Framebuffer(640, 480, false)
	{}

	Framebuffer::Framebuffer(int width, int height, bool bUseRbo)
		: m_FboID{ 0 }, m_RboID{ 0 }, m_pTexture{ nullptr }
		, m_Width{ width }, m_Height{ height }
		, m_bShouleResize{ false }, m_bUseRbo{ bUseRbo }
	{
		m_pTexture = std::move(TextureLoader::Create(Texture::TextureType::FRAMEBUFFER, width, height));
	}

	Framebuffer::~Framebuffer()
	{

	}

	void Framebuffer::Resize(int width, int height)
	{

	}

	void Framebuffer::CheckResize()
	{

	}

}