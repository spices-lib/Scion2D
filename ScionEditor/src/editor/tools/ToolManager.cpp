#include "ToolManager.h"
#include "CreateTileTool.h"
#include "ToolAccessories.h"
#include <algorithm>
#include "RectFillTool.h"
#include "editor/scene/SceneObject.h"

namespace SCION_EDITOR {

	ToolManager::ToolManager()
	{
		m_mapTools.emplace(EToolType::CREATE_TILE, std::make_unique<CreateTileTool>());
		m_mapTools.emplace(EToolType::RECT_FILE_TILE, std::make_unique<RectFillTool>());

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

		m_eActiveToolType = eToolType;
	}

	void ToolManager::SetGizmoActive(EGizmoType eGizmoType)
	{
		for (const auto& [eType, pTool] : m_mapTools)
		{
			pTool->Deactivate();
		}

		m_eActiveGizmoType = eGizmoType;
	}

	bool ToolManager::SetupTools(SceneObject* pSceneObject, SCION_RENDERING::Camera2D* pCamera)
	{
		for (auto& [eType, pTool] : m_mapTools)
		{
			if (!pTool->SetupTool(pSceneObject, pCamera))
				return false;
		}

		return true;
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