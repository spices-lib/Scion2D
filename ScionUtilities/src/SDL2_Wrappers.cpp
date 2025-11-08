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

	void SDL_Destroyer::operator()(Mix_Chunk* chunk) const
	{
		Mix_FreeChunk(chunk);
		std::cout << "Destroyed SDL Mixer\n";
	}

	void SDL_Destroyer::operator()(Mix_Music* music) const
	{
		Mix_FreeMusic(music);
		std::cout << "Destroyed SDL Music\n";
	}
}