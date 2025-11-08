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
		SDL_GameControllerClose(controller);
		controller = nullptr;
		std::cout << "Destroyed SDL Game Controller\n";
	}

	void SDL_Destroyer::operator()(SDL_Cursor* cursor) const
	{

	}
}

Controller make_shared_controller(SDL_GameController* controller)
{
	return std::shared_ptr<SDL_GameController>(controller, SCION_UTL::SDL_Destroyer{});
}

Cursor make_shared_cursor(SDL_Cursor* cursor)
{
	return std::shared_ptr<SDL_Cursor>(cursor, SCION_UTL::SDL_Destroyer{});
}