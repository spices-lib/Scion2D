#pragma once
#include <glad/glad.h>
#include <memory>
#include <Rendering/Essentials/Texture.h>

namespace SCION_RENDERING {

	class Framebuffer
	{
	private:

		GLuint m_FboID, m_RboID;
		std::shared_ptr<Texture> m_pTexture;
		int m_Width, m_Height;
		bool m_bShouleResize, m_bUseRbo;

	private:

		bool Initialize();
		void ClearUp();

	public:

		Framebuffer();
		Framebuffer(int width, int height, bool bUseRbo);
		~Framebuffer();

		void Bind();
		void Unbind();

		void Resize(int width, int height);
		void CheckResize();

		inline const GLuint GetID() const { return m_FboID; }
		inline const GLuint GetTextureID() const { return m_pTexture ? m_pTexture->GetID() : 0; }
		inline const int Width() { return m_Width; }
		inline const int Height() { return m_Height; }

	};
}