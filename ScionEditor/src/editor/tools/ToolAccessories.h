#pragma once

namespace SCION_EDITOR {

	enum class EToolType
	{
		CREATE_TILE = 0,
		RECT_FILE_TILE,

		NO_TOOL
	};

	enum class EGizmoType
	{
		TRANSLATE = 0,
		SCALE,
		ROTATE,

		NO_GIZMO
	};
}