#include "Renderer.h"
#include "Rendering/Essentials/Shader.h"
#include "Camera.h"

namespace SCION_RENDERING {

	Renderer::Renderer()
		: m_pLineBatch{ nullptr }, m_pSpriteBatch{ nullptr }
	{
		m_pLineBatch = std::make_unique<LineBatchRenderer>();
		m_pSpriteBatch = std::make_unique<SpriteBatchRenderer>();
		m_pCircleBatch = std::make_unique<CircleBatchRenderer>();
	}

	void Renderer::SetClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	void Renderer::ClearBuffers(bool color, bool depth, bool stencil)
	{
		glClear(
			(color ? GL_COLOR_BUFFER_BIT : 0) |
			(color ? GL_DEPTH_BUFFER_BIT : 0) |
			(color ? GL_STENCIL_BUFFER_BIT : 0)
		);
	}

	void Renderer::SetCapability(GLCapability capability, bool enable)
	{
		if (enable)
		{
			glEnable(static_cast<GLenum>(capability));
		}
		else
		{
			glDisable(static_cast<GLenum>(capability));
		}
	}

	bool Renderer::IsCapabilityEnable(GLCapability capability)
	{
		return glIsEnabled(static_cast<GLenum>(capability));
	}

	void Renderer::SetBlendCapability(BlendingFactors aFactor, BlendingFactors dFactor)
	{
		glBlendFunc(
			static_cast<GLenum>(aFactor),
			static_cast<GLenum>(dFactor)
		);
	}

	void Renderer::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		glViewport(x, y, width, height);
	}

	void Renderer::DrawLine(const Line& line)
	{
		m_Lines.push_back(line);
	}

	void Renderer::DrawLine(const glm::vec2 p1, const glm::vec2 p2, const Color& color, float lineWidth)
	{
		m_Lines.push_back(Line{ .p1 = p1, .p2 = p2, .color = color });
	}

	void Renderer::DrawRect(const Rect& rect)
	{
		DrawLine(Line{
			.p1 = rect.position,
			.p2 = glm::vec2{rect.position.x + rect.width, rect.position.y},
			.color = rect.color
		});

		DrawLine(Line{
			.p1 = glm::vec2{rect.position.x, rect.position.y + rect.height},
			.p2 = glm::vec2{rect.position.x + rect.width, rect.position.y + rect.height},
			.color = rect.color
		});

		DrawLine(Line{
			.p1 = rect.position,
			.p2 = glm::vec2{rect.position.x, rect.position.y + rect.height},
			.color = rect.color
		});

		DrawLine(Line{
			.p1 = glm::vec2{rect.position.x + rect.width, rect.position.y},
			.p2 = glm::vec2{rect.position.x + rect.width, rect.position.y + rect.height},
			.color = rect.color
		});
	}

	void Renderer::DrawRect(const glm::vec2 position, float width, float height, const Color& color)
	{
		DrawRect(Rect{
			.position = position,
			.width = width,
			.height = height,
			.color = color
		});
	}

	void Renderer::DrawFilledRect(const Rect& rect)
	{
	}
	void Renderer::DrawCircle(const Circle& circle)
	{
	}
	void Renderer::DrawCircle(const glm::vec3& position, float radius, const Color& color, float thickness)
	{
	}

	void Renderer::DrawText2D(const Text& text)
	{
		m_Texts.push_back(text);
	}

	void Renderer::DrawLines(Shader& shader, Camera2D& camera)
	{
		auto cam_mat = camera.GetCameraMatrix();

		shader.Enable();
		shader.SetUniformMat4("projection", cam_mat);

		m_pLineBatch->Begin();

		for (const auto& line : m_Lines)
		{
			m_pLineBatch->AddLine(line);
		}

		m_pLineBatch->End();
		m_pLineBatch->Render();

		shader.Disable();
	}

	void Renderer::DrawFilledRects(class Shader& shader, class Camera2D& camera)
	{

	}

	void Renderer::DrawCircles(class Shader& shader, class Camera2D& camera)
	{

	}

	void Renderer::DrawTexts(Shader& shader, Camera2D& camera)
	{
		if (m_Texts.empty())
			return;

		auto cam_mat = camera.GetCameraMatrix();

		shader.Enable();
		shader.SetUniformMat4("projection", cam_mat);

		m_pTextBatch->Begin();

		for (const auto& text : m_Texts)
		{
			m_pTextBatch->AddText(text.textStr, text.pFont, text.position, text.padding, text.wrap, text.color);
		}

		m_pTextBatch->End();
		m_pTextBatch->Render();

		shader.Disable();
	}

	void Renderer::ClearPrimitives()
	{
		m_Lines.clear();
		m_Circles.clear();
		m_Rects.clear();
		m_Texts.clear();
	}

}