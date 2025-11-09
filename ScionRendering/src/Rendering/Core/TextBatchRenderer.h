#pragma once
#include "Batcher.h"
#include "Rendering/Essentials/BatchTypes.h"

namespace SCION_RENDERING {

	class TextBatchRenderer : public Batcher<TextBatch, TextGlyph>
	{
	private:

		void Initialize();
		void GenerateBatches() override;

	public:

		TextBatchRenderer();
		~TextBatchRenderer() override = default;

		void End() override;
		void Render() override;

		void AddText(const std::string& text, const std::shared_ptr<Font>& font, const glm::vec2& position, 
			int padding = 4, float wrap = 0.0f, Color color = Color{ 255, 255, 255, 255 }, glm::mat4 model = glm::mat4{1.0f});
	};
}