#pragma once
#include "TileTool.h"

namespace SCION_EDITOR {

	class CreateTileTool :public TileTool
	{
	public:

		CreateTileTool();
		~CreateTileTool() override = default;

		virtual void Create() override;
		virtual void Draw() override;
	};
}