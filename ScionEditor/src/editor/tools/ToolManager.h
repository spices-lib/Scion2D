#pragma once
#include <map>
#include <memory>

namespace SCION_RENDERING {
	class Camera2D;
}

namespace SCION_EDITOR {

	struct Canvas;
	class TileTool;
	enum class EToolType;
	enum class EGizmoType;

	class ToolManager
	{
	private:

		std::map<EToolType, std::unique_ptr<TileTool>> m_mapTools;

		EToolType m_eActiveToolType;
		EGizmoType m_eActiveGizmoType;

	public:

		ToolManager();
		~ToolManager() = default;

		void Update(Canvas& canvas);

		void SetToolActive(EToolType eToolType);

		void SetGizmoActive(EGizmoType eGizmoType);

		bool SetupTools(SceneObject* pSceneObject, SCION_RENDERING::Camera2D* pCamera);

		EToolType GetActiveToolType() const { return m_eActiveToolType; }
		EGizmoType GetActiveGizmoType() const { return m_eActiveGizmoType; }

		TileTool* GetActiveTool();
	};
}