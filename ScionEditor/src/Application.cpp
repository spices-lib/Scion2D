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
#include <Core/ECS/Components/BoxColliderComponent.h>
#include <Core/ECS/Components/CircleColliderComponent.h>
#include <Core/ECS/Components/PhysicalComponent.h>
#include <Core/Resources/AssetManager.h>
#include <Core/Resources/InputManager.h>
#include <Core/Systems/ScriptingSystem.h>
#include <Core/Systems/PhysicsSystem.h>
#include <sol/sol.hpp>
#include <Core/Systems/RenderSystem.h>
#include <Core/Systems/RenderUISystem.h>
#include <Core/Systems/RenderShapeSystem.h>
#include <Core/Systems/AnimationSystem.h>
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Sounds/SoundPlayer/SoundFxPlayer.h>
#include "Rendering/Core/Renderer.h"
#include <Physics/ContactListener.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>
#include <SDL_opengl.h>

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

		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);

		m_pWindow = std::make_unique<SCION_WINDOWING::Window>(
			"Test window", 
			displayMode.w,
			displayMode.h,
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			true, 
			SDL_WINDOW_OPENGL
		);

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
			return false;
		}

		auto renderer = std::make_shared<SCION_RENDERING::Renderer>();

		auto assetManager = std::make_shared<SCION_RESOURCE::AssetManager>();

		assetManager->AddTexture("castle", "path/to/image", true);
		auto& texture = assetManager->GetTexture("castle");

		m_pRegistry = std::make_unique<SCION_CORE::ECS::Registry>();

		if (!m_pRegistry->AddToContext(renderer))
		{
			SCION_ERROR("Failed to add render to registry context!");
			return false;
		}

		auto lua = std::make_shared<sol::state>();
		lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::table, sol::lib::io, sol::lib::string);
		if (!m_pRegistry->AddToContext(lua))
		{
			SCION_ERROR("Failed to add lua state to registry context!");
			return false;
		}

		auto renderSystem = std::make_shared<SCION_CORE::Systems::RenderSystem>(*m_pRegistry);
		if (!m_pRegistry->AddToContext(renderSystem))
		{
			SCION_ERROR("Failed to add render system to registry context!");
			return false;
		}

		auto renderShapeSystem = std::make_shared<SCION_CORE::Systems::RenderShapeSystem>(*m_pRegistry);
		if (!m_pRegistry->AddToContext(renderShapeSystem))
		{
			SCION_ERROR("Failed to add render shape system to registry context!");
			return false;
		}

		auto renderUISystem = std::make_shared<SCION_CORE::Systems::RenderUISystem>(*m_pRegistry);
		if (!m_pRegistry->AddToContext(renderUISystem))
		{
			SCION_ERROR("Failed to add render ui system to registry context!");
			return false;
		}

		auto animationSystem = std::make_shared<SCION_CORE::Systems::AnimationSystem>(*m_pRegistry);
		if (!m_pRegistry->AddToContext(animationSystem))
		{
			SCION_ERROR("Failed to add animation system to registry context!");
			return false;
		}

		auto musicPlayer = std::make_shared<SCION_SOUNDS::MusicPlayer>();
		if (!musicPlayer)
		{
			SCION_ERROR("Failed to create Music Player!");
			return false;
		}

		if (!m_pRegistry->AddToContext(musicPlayer))
		{
			SCION_ERROR("Failed to add music player to registry context!");
			return false;
		}

		auto soundFxPlayer = std::make_shared<SCION_SOUNDS::SoundFxPlayer>();
		if (!soundFxPlayer)
		{
			SCION_ERROR("Failed to create soundFx Player!");
			return false;
		}

		if (!m_pRegistry->AddToContext(soundFxPlayer))
		{
			SCION_ERROR("Failed to add soundFx player to registry context!");
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

		SCION_PHYSICS::PhysicalWorld pPhysicsWorld = std::make_shared<b2World>(b2Vec2{0.0f, 9.8f});
		if (!m_pRegistry->AddToContext(pPhysicsWorld))
		{
			SCION_ERROR("Failed to add physics world to registry context!");
			return false;
		}

		auto pPhysicsSystem = std::make_shared<SCION_CORE::Systems::PhysicsSystem>(*m_pRegistry);
		if (!m_pRegistry->AddToContext(pPhysicsSystem))
		{
			SCION_ERROR("Failed to add physics system to registry context!");
			return false;
		}

		SCION_CORE::Systems::ScriptingSystem::RegisterLuaBindings(*lua, *m_pRegistry);
		SCION_CORE::Systems::ScriptingSystem::RegisterLuaFunctions(*lua);

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

		auto pContactListener = std::make_shared<SCION_PHYSICS::ContactListener>();
		if (!m_pRegistry->AddToContext(pContactListener))
		{
			SCION_ERROR("Failed to add conatct listener to registry context!");
			return false;
		}

		pPhysicsWorld->SetContactListener(pContactListener.get());

		if (!InitImGui())
		{
			SCION_ERROR("Failed to initialize ImGui!");
			return false;
		}

		LoadShaders();

		renderer->DrawLine(
			SCION_RENDERING::Line{ 
				.p1 = glm::vec2{ 50.0f }, 
				.p2 = glm::vec2{200.0f}, 
				.color = SCION_RENDERING::Color{ 255, 0, 0, 255} 
			}
		);

		return true;
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

		if (!assetMananger->AddShader("color", "assets/shaders/colorShader.vert", "assets/shaders/colorShader.vert"))
		{
			SCION_ERROR("Failed to add shader to the asset manager.");
			return false;
		}

		return true;
	}

	void Application::ProcessEvents()
	{
		auto& inputManager = SCION_CORE::InputManager::GetInstance();
		auto& keyboard = inputManager.GetKeyboard();
		auto& mouse = inputManager.GetMouse();

		while (SDL_PollEvent(&m_Event))
		{
			ImGui_ImplSDL2_ProcessEvent(&m_Event);

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
				keyboard.OnKeyPressed(m_Event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				if (m_Event.key.keysym.sym == SDLK_ESCAPE)
				{
					m_bIsRunning = false;
				}
				keyboard.OnKeyReleased(m_Event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse.OnButtonPressed(m_Event.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				mouse.OnButtonReleased(m_Event.button.button);
				break;
			case SDL_MOUSEWHEEL:
				mouse.SetMouseWheelX(m_Event.wheel.x);
				mouse.SetMouseWheelY(m_Event.wheel.y);
				break;
			case SDL_MOUSEMOTION:
				mouse.SetMouseMoving(true);
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				inputManager.GamepadButtonPressed(m_Event);
				break;
			case SDL_CONTROLLERBUTTONUP:
				inputManager.GamepadButtonReleased(m_Event);
				break;
			case SDL_CONTROLLERDEVICEADDED:
				inputManager.AddGamepad(m_Event.jdevice.which);
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				inputManager.RemoveGamepad(m_Event.jdevice.which);
				break;
			case SDL_JOYAXISMOTION:
				inputManager.GamepadAxisValues(m_Event);
				break;
			case SDL_JOYHATMOTION:
				inputManager.GamepadHatValues(m_Event);
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

		auto& pPhysicsWorld = m_pRegistry->GetContext<SCION_PHYSICS::PhysicalWorld>();
		pPhysicsWorld->Step(
			1.0f / 60.0f,
			10,
			8
		);

		auto& animationSystem = m_pRegistry->GetContext<std::shared_ptr<SCION_CORE::Systems::AnimationSystem>>();
		animationSystem->Update();

		auto& inputManager = SCION_CORE::InputManager::GetInstance();
		auto& keyboard = inputManager.GetKeyboard();
		keyboard.Update();

		auto& mouse = inputManager.GetMouse();
		mouse.Update();

		inputManager.UpdateGamepads();
	}

	void Application::Render()
	{
		auto& scriptSystem = m_pRegistry->GetContext<std::shared_ptr<SCION_CORE::Systems::ScriptingSystem>>();
		auto& renderSystem = m_pRegistry->GetContext<std::shared_ptr<SCION_CORE::Systems::RenderSystem>>();
		auto& renderUISystem = m_pRegistry->GetContext<std::shared_ptr<SCION_CORE::Systems::RenderUISystem>>();
		auto& renderShapeSystem = m_pRegistry->GetContext<std::shared_ptr<SCION_CORE::Systems::RenderShapeSystem>>();
		auto& renderer = m_pRegistry->GetContext<std::shared_ptr<SCION_RENDERING::Renderer>>();
		auto& camera = m_pRegistry->GetContext<std::shared_ptr<SCION_RENDERING::Camera2D>>();
		auto& assetManager = m_pRegistry->GetContext<std::shared_ptr<SCION_RESOURCE::AssetManager>>();

		auto& shader = assetManager->GetShader("color");

		glViewport(
			0,
			0,
			m_pWindow->GetWidth(),
			m_pWindow->GetHeight()
		);

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		scriptSystem->Render();
		renderShapeSystem->Update();
		renderSystem->Update();
		renderUISystem->Update(m_pRegistry->GetRegistry());
		renderer->DrawLines(shader, *camera);

		Begin();
		RenderImGui();
		End();

		SDL_GL_SwapWindow(m_pWindow->GetWindow().get());
	}

	void Application::ClearUp()
	{
		SDL_Quit();
	}

	bool Application::InitImGui()
	{
		const char* glslVersion = "#version 460";
		IMGUI_CHECKVERSION();

		if (!ImGui::CreateContext())
		{
			SCION_ERROR("Failed to create ImGui Context");
			return false;
		}

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.ConfigWindowsMoveFromTitleBarOnly = true;

		if (!ImGui_ImplSDL2_InitForOpenGL(
			m_pWindow->GetWindow().get(),
			m_pWindow->GetGLContext()
		))
		{
			SCION_ERROR("Failed to initialize Imgui SDL2");
			return false;
		}

		if (!ImGui_ImplOpenGL3_Init(glslVersion))
		{
			SCION_ERROR("Failed to initialize ImGui OpenGL3");
			return false;
		}

		return true;
	}

	void Application::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void Application::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			SDL_GLContext backupContext = SDL_GL_GetCurrentContext();

			SDL_GL_MakeCurrent(
				m_pWindow->GetWindow().get(),
				backupContext
			);
		}
	}

	void Application::RenderImGui()
	{
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
		ImGui::ShowDemoWindow();
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