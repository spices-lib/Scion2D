#pragma once
#include "Batcher.h"
#include "Rendering/Essentials/BatchTypes.h"
#include "Rendering/Essentials/Primitives.h"

namespace SCION_RENDERING {

	class CircleBatchRenderer : public Batcher<RectBatch, CircleGlyph>
	{
	private:

		virtual void GenerateBatches() override;
		void Initialize();

	public:

		CircleBatchRenderer();
		~CircleBatchRenderer() = default;

		virtual void End() override;
		virtual void Render() override;

		void AddCircle(const Circle& circle);
	};
}