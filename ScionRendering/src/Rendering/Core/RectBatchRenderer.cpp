#include "RectBatchRenderer.h"

namespace SCION_RENDERING {

	void RectBatchRenderer::GenerateBatches()
	{
	}

	void RectBatchRenderer::Initialize()
	{
	}

	RectBatchRenderer::RectBatchRenderer()
		: Batcher(false)
	{
		Initialize();
	}

	void RectBatchRenderer::End()
	{
		if (m_Glyphs.empty())
			return;

		GenerateBatches();
	}

	void RectBatchRenderer::Render()
	{
		EnableVAO();

		for (const auto& batch : m_Batches)
		{
			glDrawElements(GL_TRIANGLES, batch->numIndices, GL_UNSIGNED_INT, (void*)(sizeof(GLuint) * batch->offset));
		}

		DisableVAO();
	}

	void RectBatchRenderer::AddRect(const glm::vec4& dstRect, int layer, const Color& color, glm::mat4 model)
	{
		std::shared_ptr<RectGlyph> newGlyph = std::make_shared<RectGlyph>();

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.position = model * glm::vec4{ dstRect.x, dstRect.y + dstRect.w, 0.0f, 1.0f };

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.position = model * glm::vec4{ dstRect.x, dstRect.y, 0.0f, 1.0f };

		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.position = model * glm::vec4{ dstRect.x + dstRect.z, dstRect.y, 0.0f, 1.0f };

		newGlyph->topRight.color = color;
		newGlyph->topRight.position = model * glm::vec4{ dstRect.x + dstRect.z, dstRect.y + dstRect.w, 0.0f, 1.0f };

		m_Glyphs.push_back(newGlyph);
	}

	void RectBatchRenderer::AddRect(const Rect& rect, glm::mat4 model)
	{
		std::shared_ptr<RectGlyph> newGlyph = std::make_shared<RectGlyph>();

		newGlyph->topLeft.color = rect.color;
		newGlyph->topLeft.position = model * glm::vec4{ rect.position.x, rect.position.y + rect.height, 0.0f, 1.0f };

		newGlyph->bottomLeft.color = rect.color;
		newGlyph->bottomLeft.position = model * glm::vec4{ rect.position.x, rect.position.y, 0.0f, 1.0f };

		newGlyph->bottomRight.color = rect.color;
		newGlyph->bottomRight.position = model * glm::vec4{ rect.position.x + rect.width, rect.position.y, 0.0f, 1.0f };

		newGlyph->topRight.color = rect.color;
		newGlyph->topRight.position = model * glm::vec4{ rect.position.x + rect.width, rect.position.y + rect.height, 0.0f, 1.0f };

		m_Glyphs.push_back(newGlyph);
	}

}