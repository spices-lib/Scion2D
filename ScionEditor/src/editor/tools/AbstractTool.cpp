#include "AbstractTool.h"
#include <Logger.h>
#include <Core/Resources/InputManager.h>
#include <Core/ECS/Registry.h>
#include <editor/utilities/EditorFramebuffer.h>
#include <Rendering/Core/Camera.h>
#include "editor/utilities/EditorUtilities.h"

namespace SCION_EDITOR {

	void AbstractTool::UpdateMouseWorldCoords()
	{
		m_MouseScreenCoords = m_GuiCursorCoords - m_GuiRelativeCoords;
		if(!m_pCamera)
			return;

		m_MouseWorldCoords = m_pCamera->ScreenCoordsToWorld(m_MouseScreenCoords);
	}

	void AbstractTool::CheckOutOfBounds(Canvas& canvas)
	{
		auto boundsWidth{ canvas.width - (canvas.tileWidth * 0.5f)};
		auto boundsHeight{ canvas.height - (canvas.tileHeight * 0.5f)};

		if (m_WindowPos.x <= m_GuiCursorCoords.x &&
			m_WindowPos.x + m_WindowSize.x >= m_GuiCursorCoords.x &&
			m_WindowPos.y <= m_GuiCursorCoords.y &&
			m_WindowPos.y + m_WindowSize.y >= m_GuiCursorCoords.y &&
			m_MouseScreenCoords.x < m_WindowSize.x &&
			m_MouseScreenCoords.y < m_WindowSize.y &&
			m_MouseWorldCoords.x <= boundsWidth &&
			m_MouseWorldCoords.y <= boundsHeight &&
			m_MouseWorldCoords.x >= 0.0f &&
			m_MouseWorldCoords.y >= 0.0f )
		{
			m_bOutOfBounds = false;
		}
		else
		{
			m_bOutOfBounds = true;
		}
	}

	bool AbstractTool::MouseBtnJustPressed(EMouseButton eButton)
	{
		return SCION_CORE::InputManager::GetInstance().GetMouse().IsButtonJustPressed(static_cast<int>(eButton));
	}

	bool AbstractTool::MouseBtnJustReleased(EMouseButton eButton)
	{
		return SCION_CORE::InputManager::GetInstance().GetMouse().IsButtonJustReleased(static_cast<int>(eButton));
	}

	bool AbstractTool::MouseBtnPressed(EMouseButton eButton)
	{
		return SCION_CORE::InputManager::GetInstance().GetMouse().IsButtonPressed(static_cast<int>(eButton));
	}

	bool AbstractTool::MouseMoving()
	{
		return SCION_CORE::InputManager::GetInstance().GetMouse().IsMouseMoving();
	}

	AbstractTool::AbstractTool()
		: m_MouseScreenCoords{0.0f}
		, m_MouseWorldCoords{ 0.0f }
		, m_GuiCursorCoords{ 0.0f }
		, m_GuiRelativeCoords{ 0.0f }
		, m_WindowPos{ 0.0f }
		, m_WindowSize{ 0.0f }
		, m_bActivate{false}
		, m_bOutOfBounds{ false }
	{}

	void AbstractTool::Update(Canvas& canvas)
	{
		CheckOutOfBounds(canvas);
		UpdateMouseWorldCoords();
	}

	bool AbstractTool::SetupTool(SCION_CORE::ECS::Registry* pRegistry, SCION_RENDERING::Camera2D* pCamera)
	{
		if (!pRegistry)
		{
			SCION_ERROR("Failed to setup");
			return false;
		}

		if (!pCamera)
		{
			SCION_ERROR("Failed to setup");
			return false;
		}

		m_pCamera = pCamera;
		m_pRegistry = pRegistry;

		return true;
	}

}