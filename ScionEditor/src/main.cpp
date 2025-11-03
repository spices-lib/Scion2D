#define SDL_MAIN_HANDLED

#include <Windowing/Window/Window.h>
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/Vertex.h>
#include <Logger.h>
#include <Rendering/Core/Camera.h>

struct UVs
{
	float u = 0.0f;
	float v = 0.0f;
	float width = 0.0f;
	float height = 0.0f;
};

int main(int argc, char* argv[])
{
	SCION_INIT_LOGS(true, true);

	bool running = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to initialize SDL" << error << std::endl;
		running = false;
		return -1;
	}

	if (SDL_GL_LoadLibrary(NULL) != 0)
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to Open GL Library" << error << std::endl;
		running = false;
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	SCION_WINDOWING::Window window("Test window", 640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, SDL_WINDOW_OPENGL);

	if (!window.GetWindow())
	{
		std::cout << "Failed to create the window!" << std::endl;
		return -1;
	}

	window.SetGLContext(SDL_GL_CreateContext(window.GetWindow().get()));

	if (!window.GetGLContext())
	{
		std::string error = SDL_GetError();
		std::cout << "Failed to create OpenGL context: " << error << "\n";
		running = false;
		return -1;
	}

	SDL_GL_MakeCurrent(window.GetWindow().get(), window.GetGLContext());
	SDL_GL_SetSwapInterval(1);

	if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
	{
		std::cout << "Failed to LoadGL -> GLAD" << std::endl;
		running = false;
		return -1;
	}

	int width = 0;
	int height = 0;
	auto texture = SCION_RENDERING::TextureLoader::Create(SCION_RENDERING::Texture::TextureType::BLENDED, "path/to/image");
	if (!texture)
	{
		std::cout << "failed." << std::endl;
		return -1;
	}

	std::vector<SCION_RENDERING::Vertex> vertices{};
	SCION_RENDERING::Vertex vTL{}, vTR{}, vBL{}, vBR{};

	vTL.position = glm::vec2{ 10.0f, 26.0f };
	vTL.uvs = glm::vec2{};

	auto shader = SCION_RENDERING::ShaderLoader::Create("assets/shaders/basicShader.vert", "assets/shaders/basicShader.frag");
	if (!shader)
	{
		std::cout << "Create failed" << std::endl;
		return -1;
	}

	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(SCION_RENDERING::Vertex) * vertices.size(),
		vertices.data(),
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(SCION_RENDERING::Vertex),
		(void*)offsetof(SCION_RENDERING::Vertex, position)
	);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(SCION_RENDERING::Vertex),
		(void*)offsetof(SCION_RENDERING::Vertex, uvs)
	);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		4,
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		sizeof(SCION_RENDERING::Vertex),
		(void*)offsetof(SCION_RENDERING::Vertex, color)
	);

	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	SDL_Event event{};

	SCION_RENDERING::Camera2D camera;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) 
				{
					running = false;
				}
				break;
			default:
				break;
			}
		}

		glViewport(
			0,
			0,
			window.GetWidth(),
			window.GetHeight()
		);

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader->Enable();
		glBindVertexArray(VAO);

		auto projection = camera.GetCameraMatrix();

		shader->SetUniformMat4("uProjection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetID());

		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window.GetWindow().get());

		camera.Update();
		shader->Disable();
	}

	std::cout << "Closing!" << std::endl;

	return 0;
}