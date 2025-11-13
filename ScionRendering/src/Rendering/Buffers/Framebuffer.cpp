#include "Framebuffer.h"
#include <Logger.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/Texture.h>

namespace SCION_RENDERING {

	bool Framebuffer::Initialize()
	{
		glCreateFramebuffers(1, &m_FboID);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->GetID(), 0);

		if (m_bUseRbo)
		{
			glCreateRenderbuffers(1, &m_RboID);

			glBindRenderbuffer(GL_RENDERBUFFER, m_RboID);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_Width, m_Height);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERER, m_RboID);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::string error = std::to_string(glGetError());
			SCION_ERROR("Failed to create OPENGL framebuffer: {}", error);
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return false;
	}

	void Framebuffer::ClearUp()
	{
		glDeleteFramebuffers(1, &m_FboID);

		if (m_bUseRbo)
			glDeleteRenderbuffers(1, &m_RboID);

		if (m_pTexture) {
			auto textureID = m_pTexture->GetID();
			glDeleteTextures(1, &textureID);
		}
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

		if (!m_pTexture || !Initialize())
		{
			SCION_ERROR("Frame buffer creation failed");
		}
	}

	Framebuffer::~Framebuffer()
	{
		ClearUp();
	}

	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
		m_bShouleResize = true;
	}

	void Framebuffer::CheckResize()
	{
		if (!m_bShouleResize)
			return;

		ClearUp();

		m_pTexture = std::move(TextureLoader::Create(Texture::TextureType::FRAMEBUFFER, m_Width, m_Height));
		Initialize();
		m_bShouleResize = false;
	}

}