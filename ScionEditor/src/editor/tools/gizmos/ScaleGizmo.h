#pragma once
#include "Gizmo.h"

namespace SCION_EDITOR {

	class ScaleGizmo : public Gizmo
	{
	public:

		ScaleGizmo();

		virtual void Update(Canvas& canvas) override;
		virtual void Draw(Canvas& canvas) override;
	};

}