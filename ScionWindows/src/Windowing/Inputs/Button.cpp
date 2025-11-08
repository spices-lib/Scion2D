#include "Button.h"

namespace SCION_WINDOWING::Inputs {

	void Button::Update(bool bPressed)
	{
		bJustPressed = !bIsPressed && bPressed;
		bJustReleased = bIsPressed && !bPressed;
		bIsPressed = bPressed;
	}

	void Button::Reset()
	{
		bJustPressed = false;
		bJustReleased = false;
	}
}