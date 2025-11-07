#include "Button.h"

namespace SCION_WINDOWING::Inputs {

	void Button::Update(bool bPressed)
	{
		bJustPressed = !bIsPressed && bPressed;
		bJusyReleased = bIsPressed && !bPressed;
		bIsPressed = bPressed;
	}

	void Button::Reset()
	{
		bJustPressed = false;
		bJusyReleased = false;
	}
}