#pragma once
#include "Button.h"
#include "MouseButton.h"
#include <map>

namespace SCION_WINDOWING::Inputs {

	class Mouse
	{
	private:

		std::map<int, Button> m_mapButtons
		{
			{SCION_MOUSE_LEFT, Button{}},
			{SCION_MOUSE_MIDDLE, Button{}},
			{SCION_MOUSE_RIGHT, Button{}}
		};

		int m_X{ 0 }, m_Y{ 0 }, m_WheelX{ 0 }, m_WheelY{ 0 };
		bool m_bMouseMoving{ false };

	public:

		Mouse() = default;
		~Mouse() = default;

		void Update();
		void OnButtonPressed(int btn);
		void OnButtonReleased(int btn);

		const bool IsButtonPressed(int btn);
		const bool IsButtonJustPressed(int btn);
		const bool IsButtonJustReleased(int btn);

		std::tuple<int, int> GetMouseScreenPosition();

		inline void SetMouseWheelX(int wheel) { m_WheelX = wheel; }
		inline void SetMouseWheelY(int wheel) { m_WheelY = wheel; }
		inline void SetMouseMoving(bool moving) { m_bMouseMoving = moving; }

		inline int GetMouseWheelX() { return m_WheelX; }
		inline int GetMouseWheelY() { return m_WheelY; }
	};
}