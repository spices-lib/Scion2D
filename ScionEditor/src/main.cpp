#define SDL_MAIN_HANDLED

#include "Application.h"

int main(int argc, char* argv[])
{
	SCION_EDITOR::Application::GetInstance().Run();

	return 0;
}