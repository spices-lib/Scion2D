#pragma once
#include <map>
#include <memory>

namespace SCION_EDITOR {

	struct Canvas;
	class TileTool;
	enum class EToolType;

	class ToolManager
	{
	private:

		std::map<EToolType, std::unique_ptr<TileTool>> m_mapTools;

	public:

		ToolManager();
		~ToolManager() = default;

		void Update(Canvas& canvas);

		void SetToolActive(EToolType eToolType);

		TileTool* GetActiveTool();
	};
}