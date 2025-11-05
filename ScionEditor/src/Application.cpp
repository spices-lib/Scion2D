#include "Application.h"
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
#include <Core/ECS/Entity.h>
#include <Core/ECS/Components/SpriteComponent.h>
#include <Core/ECS/Components/TransformComponent.h>
#include <Core/ECS/Components/Identification.h>
#include <Core/Resources/AssetManager.h>
#include <Core/Systems/ScriptingSystem.h>
#include <sol/sol.hpp>

namespace SCION_EDITOR {

	bool Application::Initialize()
	{
		SCION_INIT_LOGS(true, true);

		m_bIsRunning = true;

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::string error = SDL_GetError();
			std::cout << "Failed to initialize SDL" << error << std::endl;
			m_bIsRunning = false;
			return false;
		}

		if (SDL_GL_LoadLibrary(NULL) != 0)
		{
			std::string error = SDL_GetError();
			std::cout << "Failed to Open GL Library" << error << std::endl;
			m_bIsRunning = false;
			return false;
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

		m_pWindow = std::make_unique<SCION_WINDOWING::Window>("Test window", 640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, true, SDL_WINDOW_OPENGL);

		if (!m_pWindow->GetWindow())
		{
			std::cout << "Failed to create the window!" << std::endl;
			return false;
		}

		m_pWindow->SetGLContext(SDL_GL_CreateContext(m_pWindow->GetWindow().get()));

		if (!m_pWindow->GetGLContext())
		{
			std::string error = SDL_GetError();
			std::cout << "Failed to create OpenGL context: " << error << "\n";
			m_bIsRunning = false;
			return false;
		}

		SDL_GL_MakeCurrent(m_pWindow->GetWindow().get(), m_pWindow->GetGLContext());
		SDL_GL_SetSwapInterval(1);

		if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0)
		{
			std::cout << "Failed to LoadGL -> GLAD" << std::endl;
			m_bIsRunning = false;
			return -1;
		}

		auto assetManager = std::make_shared<SCION_RESOURCE::AssetManager>();

		assetManager->AddTexture("castle", "path/to/image", true);
		auto& texture = assetManager->GetTexture("castle");

		m_pRegistry = std::make_unique<SCION_CORE::ECS::Registry>();

		SCION_CORE::ECS::Entity entity{ *m_pRegistry, "Ent1", "Test" };
		auto& transform = entity.AddComponent<SCION_CORE::ECS::TransformComponent>(SCION_CORE::ECS::TransformComponent{
			.position = glm::vec2{ 10.0f, 10.0f },
			.scale = glm::vec2{ 1.0f, 1.0f },
			.rotation = 0.0f
		});

		auto& sprite = entity.AddComponent<SCION_CORE::ECS::SpriteComponent>(SCION_CORE::ECS::SpriteComponent{
			.width = 16.0f,
			.height = 16.0f,
			.color = SCION_RENDERING::Color{ .r = 255, .g = 0, .b = 255, .a = 255 },
			.start_x = 0,
			.start_y = 28
		});

		sprite.generate_uvs(texture.GetWidth(), texture.GetHeight());

		auto& id = entity.GetComponent<SCION_CORE::ECS::Identification>();

		std::vector<SCION_RENDERING::Vertex> vertices{};
		SCION_RENDERING::Vertex vTL{}, vTR{}, vBL{}, vBR{};

		vTL.position = glm::vec2{ 10.0f, 26.0f };
		vTL.uvs = glm::vec2{};

		auto lua = std::make_shared<sol::state>();
		lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::table, sol::lib::io, sol::lib::string);
		if (!m_pRegistry->AddToContext(lua))
		{
			SCION_ERROR("Failed to add lua state to registry context!");
			return false;
		}

		auto scriptSystem = std::make_shared<SCION_CORE::Systems::ScriptingSystem>(*m_pRegistry);
		if (!scriptSystem->LoadMainScript(*lua))
		{
			SCION_ERROR("Failed to load main lua script.")
			return false;
		}
		if (!m_pRegistry->AddToContext(scriptSystem))
		{
			SCION_ERROR("Failed to add script system to registry context!");
			return false;
		}

		auto camera = std::make_shared<SCION_RENDERING::Camera2D>();
		camera->SetScale(15.0f);

		if (!m_pRegistry->AddToContext(assetManager))
		{
			SCION_ERROR("Failed to add asset mananger to registry context!");
			return false;
		}

		if (!m_pRegistry->AddToContext(camera))
		{
			SCION_ERROR("Failed to add camera to registry context!");
			return false;
		}

		LoadShaders();

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

		
	}

	bool Application::LoadShaders()
	{
		auto& assetMananger = m_pRegistry->GetContext<std::shared_ptr<SCION_RESOURCE::AssetManager>>();

		if (!assetMananger)
		{
			SCION_ERROR("Failed to get asset mananger from registry context!");
			return false;
		}

		if (!assetMananger->AddShader("basic", "assets/shaders/basicShader.vert", "assets/shaders/basicShader.vert"))
		{
			SCION_ERROR("Failed to add shader to the asset manager.");
			return false;
		}

		return true;
	}

	void Application::ProcessEvents()
	{
		while (SDL_PollEvent(&m_Event))
		{
			switch (m_Event.type)
			{
			case SDL_QUIT:
				m_bIsRunning = false;
				break;
			case SDL_KEYDOWN:
				if (m_Event.key.keysym.sym == SDLK_ESCAPE)
				{
					m_bIsRunning = false;
				}
				break;
			default:
				break;
			}
		}
	}

	void Application::Update()
	{
		auto& camera = m_pRegistry->GetContext<std::shared_ptr<SCION_RENDERING::Camera2D>>();

		if (!camera)
		{
			SCION_ERROR("Failed to get camera from registry context!");
		}

		camera->Update();

		auto& scriptSystem = m_pRegistry->GetContext<std::shared_ptr<SCION_CORE::Systems::ScriptingSystem>>();
		scriptSystem->Update();
	}

	void Application::Render()
	{
		auto& assetMananger = m_pRegistry->GetContext<std::shared_ptr<SCION_RESOURCE::AssetManager>>();

		if (!assetMananger)
		{
			SCION_ERROR("Failed to get asset mananger from registry context!");
			return;
		}

		auto& camera = m_pRegistry->GetContext<std::shared_ptr<SCION_RENDERING::Camera2D>>();

		if (!camera)
		{
			SCION_ERROR("Failed to get camera from registry context!");
			return;
		}

		auto& shader = assetMananger->GetShader("basic");
		
		if (shader.ShaderProgramID() == 0)
		{
			SCION_ERROR("Shader not valid");
			return;
		}

		glViewport(
			0,
			0,
			m_pWindow->GetWidth(),
			m_pWindow->GetHeight()
		);

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.Enable();
		glBindVertexArray(VAO);

		auto& projection = camera->GetCameraMatrix();

		shader.SetUniformMat4("uProjection", projection);

		auto& texture = assetMananger->GetTexture("castle");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.GetID());

		auto& scriptSystem = m_pRegistry->GetContext<std::shared_ptr<SCION_CORE::Systems::ScriptingSystem>>();
		scriptSystem->Render();

		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(m_pWindow->GetWindow().get());

		shader.Disable();
	}

	void Application::ClearUp()
	{
		SDL_Quit();
	}

	Application::Application()
		: m_pWindow{ nullptr }
		, m_pRegistry{ nullptr }
		, m_Event{}
		, m_bIsRunning{ true }
	{}

	Application& Application::GetInstance()
	{
		static Application app{};
		return app;
	}

	Application::~Application()
	{}

	void Application::Run()
	{
		if (!m_bIsRunning)
		{
			return;
		}

		while (m_bIsRunning)
		{
			ProcessEvents();
			Update();
			Render();
		}
	}

}