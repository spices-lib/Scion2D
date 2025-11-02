#define SDL_MAIN_HANDLED

#include <Windowing/Window/Window.h>
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
private:
	int m_Width;
	int m_Height;
	float m_Scale;

	glm::vec2 m_Position;
	glm::mat4 m_CameraMatrix, m_OrthoProjection;

	bool m_bNeedsUpdate;

public:

	Camera2D()
		: Camera2D(640, 480)
	{}

	Camera2D(int width, int height)
		: m_Width(width), m_Height(height), m_Scale(1.0f)
		, m_Position(glm::vec2(0.0f))
		, m_CameraMatrix(1.0f)
		, m_OrthoProjection(1.0f)
		, m_bNeedsUpdate(true)
	{
		m_OrthoProjection = glm::ortho(0.0f, static_cast<float>(m_Width), 0.0f, static_cast<float>(m_Height), -1.0f, 1.0f);
	}

	inline glm::mat4 GetCameraMatrix() { return m_CameraMatrix; }
	void Update()
	{
		if (!m_bNeedsUpdate)
			return;

		glm::vec3 translate(-m_Position.x, -m_Position.y, 0.0f);
		m_CameraMatrix = glm::translate(m_OrthoProjection, translate);

		glm::vec3 scale(m_Scale, m_Scale, 0.0f);
		m_CameraMatrix *= glm::scale(glm::mat4(1.0f), scale);

		m_bNeedsUpdate = false;
	}
};

struct UVs
{
	float u = 0.0f;
	float v = 0.0f;
	float width = 0.0f;
	float height = 0.0f;
};

bool LoadTexture(const std::string& filePath, int& width, int& height, bool blended)
{
	int channels = 0;

	auto* image = stbi_load(
		filePath.c_str(),
		&width,
		&height,
		&channels,
		4
	);

	if (!image)
	{
		std::cout << "SOLT failed to load image [" << filePath << "]";
		return false;
	}

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (!blended)
	{
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);

	return true;
}

int main(int argc, char* argv[])
{
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

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	int width = 0;
	int height = 0;
	if (!LoadTexture("path/to/image", width, height, false))
	{
		std::cout << "failed." << std::endl;
		return -1;
	}

	float vertices[] =
	{
		 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	const char* vertexSource = "";

	GLuint vertexShader{};
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	int status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Failed to compile vertex shader!" << infoLog << std::endl;
		return -1;
	}
		
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, vertexShader);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

	if (!status)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to link shader program" << infoLog << std::endl;
		return -1;
	}

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(vertexShader);

	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vertices) * 3 * sizeof(float),
		vertices,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		(void*)0
	);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	SDL_Event event{};

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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);


		SDL_GL_SwapWindow(window.GetWindow().get());
	}

	std::cout << "Closing!" << std::endl;

	return 0;
}