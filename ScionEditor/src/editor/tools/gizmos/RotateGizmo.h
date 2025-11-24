#pragma once
#include "Gizmo.h"

namespace SCION_EDITOR {

	class RotateGizmo : public Gizmo
	{
	public:

		RotateGizmo();

		virtual void Update(Canvas& canvas) override;
		virtual void Draw(Canvas& canvas) override;
	};

}