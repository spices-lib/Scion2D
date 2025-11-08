#pragma once
#include <SDL.h>
#include <memory>

namespace SCION_UTL {

	struct SDL_Destroyer
	{
		void operator()(SDL_Window* window) const;
		void operator()(SDL_GameController* controller) const;
		void operator()(SDL_Cursor* cursor) const;
	};

}

typedef std::shared_ptr<SDL_GameController> Controller;
static Controller make_shared_controller(SDL_GameController* controller)
{
	return std::shared_ptr<SDL_GameController>(controller, SCION_UTL::SDL_Destroyer{});
}

typedef std::shared_ptr<SDL_Cursor> Cursor;
static Cursor make_shared_cursor(SDL_Cursor* cursor)
{
	return std::shared_ptr<SDL_Cursor>(cursor, SCION_UTL::SDL_Destroyer{});
}

typedef std::unique_ptr<SDL_Window, SCION_UTL::SDL_Destroyer> WindowPtr;

