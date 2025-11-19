#pragma once
#include "IDisplay.h"

namespace SCION_EDITOR {

	class MenuDisplay : public IDisplay
	{
	private:



	public:

		MenuDisplay() = default;
		~MenuDisplay() = default;

		virtual void Draw() override;

	};
}