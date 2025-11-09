#pragma once
#include "Rendering/Essentials/Vertex.h"
#include "Batcher.h"
#include "Rendering/Essentials/BatchTypes.h"
#include <vector>
#include <memory>

namespace SCION_RENDERING {

	class SpriteBatchRenderer : public Batcher<Batch, SpriteGlyph>
	{
	private:

		void Initialize();
		void GenerateBatches() override;

	public:

		SpriteBatchRenderer();
		~SpriteBatchRenderer() override = default;

		void End() override;
		void Render() override;

		void AddSprite(const glm::vec4& spriteRect, const glm::vec4 uvRect, GLuint textureID, int layer = 0, 
			glm::mat4 model = glm::mat4{ 1.0f }, const Color& color = Color{.r = 255, .g = 255, .b = 255, .a = 255});
	};

}