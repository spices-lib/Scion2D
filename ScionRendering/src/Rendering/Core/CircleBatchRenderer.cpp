#include "CircleBatchRenderer.h"

namespace SCION_RENDERING {

	void CircleBatchRenderer::GenerateBatches()
	{
		std::vector<CircleVertex> vertices;
		vertices.resize(m_Glyphs.size() * NUM_SPRITE_VERTICES);

		int currentVertex{ 0 }, currentCircle{ 0 };
		GLuint offset{ 0 };

		for (const auto& circle : m_Glyphs)
		{
			if (currentCircle == 0)
			{
				m_Batches.push_back(std::make_shared<RectBatch>(RectBatch{
					.numIndices = NUM_SPRITE_INDICES, .offset = offset
				}));
			}
			else
			{
				m_Batches.back()->numIndices += NUM_SPRITE_INDICES;
			}

			vertices[currentVertex++] = circle->topLeft;
			vertices[currentVertex++] = circle->topRight;
			vertices[currentVertex++] = circle->bottomRight;
			vertices[currentVertex++] = circle->bottomLeft;

			offset += NUM_SPRITE_INDICES;
			currentCircle++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, GetVBO());

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(CircleVertex), nullptr, GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(CircleVertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void CircleBatchRenderer::Initialize()
	{
		SetVertexAttribute(0, 2, GL_FLOAT, sizeof(CircleVertex), (void*)offsetof(CircleVertex, position));
		SetVertexAttribute(1, 2, GL_FLOAT, sizeof(CircleVertex), (void*)offsetof(CircleVertex, uvs));
		SetVertexAttribute(2, 4, GL_UNSIGNED_BYTE, sizeof(CircleVertex), (void*)offsetof(CircleVertex, color), GL_TRUE);
		SetVertexAttribute(3, 1, GL_FLOAT, sizeof(CircleVertex), (void*)offsetof(CircleVertex, lineThickness), GL_TRUE);
	}

	CircleBatchRenderer::CircleBatchRenderer()
		: Batcher(false)
	{
		Initialize();
	}

	void CircleBatchRenderer::End()
	{
		if (m_Glyphs.empty())
			return;

		GenerateBatches();
	}

	void CircleBatchRenderer::Render()
	{
		glEnable(GL_LINE_SMOOTH);

		EnableVAO();

		for (const auto& batch : m_Batches)
		{
			glDrawElements(GL_TRIANGLES, batch->numIndices, GL_UNSIGNED_INT, (void*)(sizeof(GLuint) * batch->offset));
		}

		DisableVAO();
	}

	void CircleBatchRenderer::AddCircle(const Circle& circle)
	{
		glm::mat4 model{ 1.0f };
		std::shared_ptr<CircleGlyph> newCircle = std::make_shared<CircleGlyph>(CircleGlyph{
			.topLeft = CircleVertex {
				.position = model * glm::vec4{ circle.position.x, circle.position.y + circle.radius, 0.0f, 1.0f },
				.uvs = glm::vec2{ 1.0f, 1.0f },
				.color = circle.color,
				.lineThickness = circle.thickness
			},
			.bottomLeft = CircleVertex {
				.position = model * glm::vec4{ circle.position.x, circle.position.y, 0.0f, 1.0f },
				.uvs = glm::vec2{ 1.0f, -1.0f },
				.color = circle.color,
				.lineThickness = circle.thickness
			},
			.topRight = CircleVertex {
				.position = model * glm::vec4{ circle.position.x + circle.radius, circle.position.y + circle.radius, 0.0f, 1.0f },
				.uvs = glm::vec2{ -1.0f, 1.0f },
				.color = circle.color,
				.lineThickness = circle.thickness
			},
			.bottomRight = CircleVertex {
				.position = model * glm::vec4{ circle.position.x + circle.radius, circle.position.y, 0.0f, 1.0f },
				.uvs = glm::vec2{ -1.0f, -1.0f },
				.color = circle.color,
				.lineThickness = circle.thickness
			}
		});

		m_Glyphs.push_back(newCircle);
	}

}