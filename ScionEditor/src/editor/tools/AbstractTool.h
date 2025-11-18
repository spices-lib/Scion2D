#pragma once
#include <glm/glm.hpp>
#include <Core/ECS/MainRegistry.h>

namespace SCION_RENDERING {
	class Camera2D;
}

namespace SCION_EDITOR {

	struct Canvas;

	class AbstractTool
	{
	private:

		glm::vec2 m_MouseScreenCoords, m_MouseWorldCoords;
		glm::vec2 m_GuiCursorCoords, m_GuiRelativeCoords;
		glm::vec2 m_WindowPos, m_WindowSize;

		bool m_bActivate, m_bOutOfBounds, m_bOverTilemap;

	private:

		void UpdateMouseWorldCoords();
		void CheckOutOfBounds(Canvas& canvas);

	protected:

		enum class EMouseButton
		{
			UNKNOWN = 0,
			LEFT,
			MIDDLE,
			RIGHT,

			LAST
		};

		SCION_CORE::ECS::Registry* m_pRegistry{ nullptr };
		SCION_RENDERING::Camera2D* m_pCamera{ nullptr };

	protected:

		bool MouseBtnJustPressed(EMouseButton eButton);
		bool MouseBtnJustReleased(EMouseButton eButton);
		bool MouseBtnPressed(EMouseButton eButton);
		bool MouseMoving();

		virtual void ExamineMousePosition() = 0;

		inline void SetMouseWorldCoords(const glm::vec2& newCoords) { m_MouseWorldCoords = newCoords; }

	public:

		AbstractTool();
		virtual ~AbstractTool() = default;

		virtual void Update(Canvas& canvas);
		bool SetupTool(SCION_CORE::ECS::Registry* pRegistry, SCION_RENDERING::Camera2D* pCamera);

		inline void SetRelativeCoords(const glm::vec2& relativeCoords) { m_GuiRelativeCoords = relativeCoords; }
		inline void SetCursorCoords(const glm::vec2& cursorCoords) { m_GuiCursorCoords = cursorCoords; }
		inline void SetWindowPos(const glm::vec2& windowPos) { m_WindowPos = windowPos; }
		inline void SetWindowSize(const glm::vec2& windowSize) { m_WindowSize = windowSize; }

		inline const glm::vec2& GetMouseScreenCoords() const { return m_MouseScreenCoords; }
		inline const glm::vec2& GetMouseWorldCoords() const { return m_MouseWorldCoords; }

		inline void Activate() { m_bActivate = true; }
		inline void Deactivate() { m_bActivate = false; }
		inline void SetOverTilemapWindow(bool bOverWindow) { m_bOverTilemap = bOverWindow; }
		inline const bool IsActivated() const { return m_bActivate; }
		inline const bool OutOfBounds() const { return m_bOutOfBounds; }
		inline const bool IsOverTilemapWindow() const { return m_bOverTilemap; }

	};
}