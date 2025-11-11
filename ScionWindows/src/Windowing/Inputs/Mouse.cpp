#include "Mouse.h"
#include <SDL.h>
#include <Logger.h>

namespace SCION_WINDOWING::Inputs {

	void Mouse::Update()
	{
		for (auto& [btn, button] : m_mapButtons)
		{
			button.Reset();
		}

		m_WheelX = 0;
		m_WheelY = 0;
		m_bMouseMoving = false;
	}

	void Mouse::OnButtonPressed(int btn)
	{
		if (btn == SCION_MOUSE_UNKNOWN)
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return;
		}

		if (!m_mapButtons.contains(btn))
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return;
		}

		m_mapButtons.at(btn).Update(true);
	}

	void Mouse::OnButtonReleased(int btn)
	{
		if (btn == SCION_MOUSE_UNKNOWN)
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return;
		}

		if (!m_mapButtons.contains(btn))
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return;
		}

		m_mapButtons.at(btn).Update(false);
	}

	const bool Mouse::IsButtonPressed(int btn)
	{
		if (btn == SCION_MOUSE_UNKNOWN)
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return false;
		}

		if (!m_mapButtons.contains(btn))
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return false;
		}

		return m_mapButtons.at(btn).bIsPressed;
	}

	const bool Mouse::IsButtonJustPressed(int btn)
	{
		if (btn == SCION_MOUSE_UNKNOWN)
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return false;
		}

		if (!m_mapButtons.contains(btn))
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return false;
		}

		return m_mapButtons.at(btn).bJustPressed;
	}

	const bool Mouse::IsButtonJustReleased(int btn)
	{
		if (btn == SCION_MOUSE_UNKNOWN)
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return false;
		}

		if (!m_mapButtons.contains(btn))
		{
			SCION_ERROR("Mouse Button {} is unknown", btn);
			return false;
		}

		return m_mapButtons.at(btn).bJustReleased;
	}

	glm::vec2 Mouse::GetMouseScreenPosition()
	{
		SDL_GetMouseState(&m_X, &m_Y);
		return glm::vec2(m_X, m_Y);
	}

}