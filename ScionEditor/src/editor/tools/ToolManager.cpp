#include "ToolManager.h"
#include "CreateTileTool.h"
#include "ToolAccessories.h"
#include <algorithm>

namespace SCION_EDITOR {

	ToolManager::ToolManager()
	{
		auto pCreateTileTool = std::make_unique<CreateTileTool>();
		m_mapTools.emplace(EToolType::CREATE_TILE, std::move(pCreateTileTool));

		SetToolActive(EToolType::CREATE_TILE);
	}

	void ToolManager::Update(Canvas& canvas)
	{
		auto activeTool = std::ranges::find_if(m_mapTools, [](const auto& tool) { return tool.second->IsActivated(); });
		if (activeTool != m_mapTools.end())
		{
			activeTool->second->Update(canvas);
		}
	}

	void ToolManager::SetToolActive(EToolType eToolType)
	{
		for (const auto& [eType, pTool] : m_mapTools)
		{
			if (eType == eToolType)
				pTool->Activate();
			else
				pTool->Deactivate();
		}
	}

	TileTool* ToolManager::GetActiveTool()
	{
		const auto activeTool = std::ranges::find_if(m_mapTools, [](const auto& tool) { return tool.second->IsActivated(); });
		if (activeTool != m_mapTools.end())
		{
			return activeTool->second.get();
		}

		return nullptr;
	}
}