#pragma once
#include <SDL2_Wrappers.h>
#include <string>

namespace SCION_WINDOWING {

	class Window
	{
	public:
		Window() : Window("default_window", 640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, NULL) {}
		Window(const std::string& title, int width, int height, int x_pox, int y_pox, bool v_sync = true,
			uint32_t flags = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_CAPTURE));
		virtual ~Window();

		inline void SetGLContext(SDL_GLContext gl_context) { m_GLContext = gl_context; }
		inline SDL_GLContext& GetGLContext() { return m_GLContext; }
		inline WindowPtr& GetWindow() { return m_pWindow; }
		void SetWindowName(const std::string& name);
		inline const std::string& GetWindowName() const { return m_sTitle; }
		inline const int GetXPos() const { return m_XPos; }
		inline const int GetYPos() const { return m_YPos; }
		inline const int SetXPos(int x_pos) { m_XPos = x_pos; }
		inline const int SetYPos(int y_pos) { m_YPos = y_pos; }
		inline const int GetWidth() const { return m_Width; }
		inline const int GetHeight() const { return m_Height; }

	private:

		void CreateNewWindow(uint32_t flags);

	private:

		WindowPtr m_pWindow;
		SDL_GLContext m_GLContext;
		std::string m_sTitle;
		int m_Width, m_Height, m_XPos, m_YPos;
		uint32_t m_WindowFlags;
	};

}
