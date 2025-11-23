#pragma once
#include "Gizmo.h"

namespace SCION_EDITOR {

	class TranslateGizmo : public Gizmo
	{
	public:

		TranslateGizmo();

		virtual void Update(Canvas& canvas) override;
		virtual void Draw(Canvas& canvas) override;
	};
}