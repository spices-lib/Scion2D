#pragma once
#include <map>
#include <memory>

namespace SCION_RENDERING {
	class Camera2D;
}

namespace SCION_EDITOR {

	struct Canvas;
	class TileTool;
	class Gizmo;
	enum class EToolType;
	enum class EGizmoType;
	class AbstractTool;

	class ToolManager
	{
	private:

		std::map<EToolType, std::unique_ptr<TileTool>> m_mapTools;
		std::map<EGizmoType, std::unique_ptr<Gizmo>> m_mapGizmos;

		EToolType m_eActiveToolType;
		EGizmoType m_eActiveGizmoType;

	public:

		ToolManager();
		~ToolManager() = default;

		void Update(Canvas& canvas);

		void SetToolActive(EToolType eToolType);

		void SetGizmoActive(EGizmoType eGizmoType);

		bool SetupTools(class SceneObject* pSceneObject, SCION_RENDERING::Camera2D* pCamera);

		EToolType GetActiveToolType() const { return m_eActiveToolType; }
		EGizmoType GetActiveGizmoType() const { return m_eActiveGizmoType; }

		TileTool* GetActiveTool();
		Gizmo* GetActiveGizmo();

		AbstractTool* GetActiveToolFromAbstract();
	};
}