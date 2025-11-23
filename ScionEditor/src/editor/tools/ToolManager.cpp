#include "ToolManager.h"
#include "CreateTileTool.h"
#include "ToolAccessories.h"
#include <algorithm>
#include "RectFillTool.h"
#include "editor/scene/SceneObject.h"
#include "editor/tools/gizmos/TranslateGizmo.h"

namespace SCION_EDITOR {

	ToolManager::ToolManager()
	{
		m_mapTools.emplace(EToolType::CREATE_TILE, std::make_unique<CreateTileTool>());
		m_mapTools.emplace(EToolType::RECT_FILE_TILE, std::make_unique<RectFillTool>());

		m_mapGizmos.emplace(EGizmoType::TRANSLATE, std::make_unique<TranslateGizmo>());

		SetToolActive(EToolType::CREATE_TILE);
	}

	void ToolManager::Update(Canvas& canvas)
	{
		auto activeTool = std::ranges::find_if(m_mapTools, [](const auto& tool) { return tool.second->IsActivated(); });
		if (activeTool != m_mapTools.end())
		{
			activeTool->second->Update(canvas);
		}

		auto activeGizmo = std::ranges::find_if(m_mapGizmos, [](const auto& tool) { return tool.second->IsActivated(); });
		if (activeGizmo != m_mapGizmos.end())
		{
			activeGizmo->second->Update(canvas);
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
		for (const auto& [eType, pTool] : m_mapGizmos)
		{
			if (eType == eGizmoType)
				pTool->Activate();
			else
				pTool->Deactivate();
		}

		m_eActiveGizmoType = eGizmoType;
	}

	bool ToolManager::SetupTools(SceneObject* pSceneObject, SCION_RENDERING::Camera2D* pCamera)
	{
		for (auto& [eType, pTool] : m_mapTools)
		{
			if (!pTool->SetupTool(nullptr, pCamera))
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
	Gizmo* ToolManager::GetActiveGizmo()
	{
		const auto activeGizmo = std::ranges::find_if(m_mapGizmos, [](const auto& tool) { return tool.second->IsActivated(); });
		if (activeGizmo != m_mapGizmos.end())
		{
			return activeGizmo->second.get();
		}

		return nullptr;
	}

	AbstractTool* ToolManager::GetActiveToolFromAbstract()
	{
		if (auto* pTool = GetActiveTool())
			return pTool;

		if (auto* pGizmo = GetActiveGizmo())
			return pGizmo;

		return nullptr;
	}
}