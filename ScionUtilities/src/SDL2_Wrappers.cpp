#include "SDL2_Wrappers.h"
#include <iostream>

namespace SCION_UTL {

	void SDL_Destroyer::operator()(SDL_Window * window) const
	{
		SDL_DestroyWindow(window);
		std::cout << "Destroyed SDL Window\n";
	}

	void SDL_Destroyer::operator()(SDL_GameController* controller) const
	{

	}

	void SDL_Destroyer::operator()(SDL_Cursor* cursor) const
	{

	}
}

Controller make_shared_controller(SDL_GameController* controller)
{
	return std::shared_ptr<SDL_GameController>(controller);
}

Cursor make_shared_cursor(SDL_Cursor* cursor)
{
	return std::shared_ptr<SDL_Cursor>(cursor);
}