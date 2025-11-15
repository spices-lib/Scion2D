#include "SceneDisplay.h"
#include <imgui.h>
#include <Rendering/Buffers/Framebuffer.h>
#include <Core/ECS/MainRegistry.h>
#include <Core/Systems/AnimationSystem.h>
#include <Core/Systems/PhysicsSystem.h>
#include <Core/Systems/RenderShapeSystem.h>
#include <Core/Systems/RenderSystem.h>
#include <Core/Systems/RenderUISystem.h>
#include <Core/Systems/ScriptingSystem.h>
#include <Logger.h>
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Sounds/SoundPlayer/SoundFxPlayer.h>
#include <Physics/Box2DWrappers.h>
#include <Core/CoreUtilities/CoreEngineData.h>
#include <Core/Resources/AssetManager.h>
#include <imgui.h>

using namespace SCION_CORE::Systems;

namespace SCION_EDITOR {

	void SceneDisplay::LoadScene()
	{
		auto& scriptSystem = m_Registry.GetContext<std::shared_ptr<ScriptingSystem>>();

		auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();
		lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::table, sol::lib::io, sol::lib::string);

		SCION_CORE::Systems::ScriptingSystem::RegisterLuaBindings(*lua, m_Registry);
		SCION_CORE::Systems::ScriptingSystem::RegisterLuaFunctions(*lua);

		if (!scriptSystem->LoadMainScript(*lua))
		{
			SCION_ERROR("Failed to load main lua script.");
			return;
		}

		m_bSceneLoaded = true;
		m_bPlayScene = true;
	}

	void SceneDisplay::UnloadScene()
	{
		m_bPlayScene = false;
		m_bSceneLoaded = false;
		m_Registry.GetRegistry().clear();
		auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();
		lua.reset();

		auto& mainRegistry = SCION_CORE::ECS::MainRegistry::GetInstance();
		mainRegistry.GetMusicPlayer().Stop();
		mainRegistry.GetSoundPlayer().Stop(-1);
	}

	SceneDisplay::SceneDisplay(SCION_CORE::ECS::Registry& registry)
		: m_Registry(registry)
		, m_bPlayScene{ false }
		, m_bSceneLoaded{ false }
	{}

	void SceneDisplay::Draw()
	{
		static bool pOpen{ true };
		if (!ImGui::Begin("Scene", &pOpen))
		{
			ImGui::End();
			return;
		}

		auto& mainRegistry = SCION_CORE::ECS::MainRegistry::GetInstance();
		auto& assetManager = mainRegistry.GetAssetManager();

		auto& playTexture = assetManager.GetTexture("play_button");
		auto& stopTexture = assetManager.GetTexture("stop_button");

		if (m_bPlayScene)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.9f, 0.0f, 0.3f});
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.0f, 0.9f, 0.0f, 0.3f});
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.0f, 0.9f, 0.0f, 0.3f});
		}

		if (ImGui::ImageButton("##", playTexture.GetID(), ImVec2{ playTexture.GetWidth() , playTexture.GetHeight() }))
		{
			LoadScene();
		}

		if (m_bPlayScene)
		{
			ImGui::PopStyleColor(3);
		}

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
		{
			ImGui::SetTooltip("Play Scene");
		}

		ImGui::SameLine();

		if (ImGui::ImageButton("##", stopTexture.GetID(), ImVec2{ stopTexture.GetWidth() , stopTexture.GetHeight() }))
		{
			UnloadScene();
		}

		if (ImGui::BeginChild("##SceneChild", ImVec2{ 0.0f, 0.0f }, NULL, ImGuiWindowFlags_NoScrollWithMouse))
		{
			const auto& fb = m_Registry.GetContext<std::shared_ptr<SCION_RENDERING::Framebuffer>>();
	
			ImGui::SetCursorPos(ImVec2{ 0.0f, 0.0f });

			ImGui::Image(fb->GetTextureID(), ImVec2{ (float)fb->Width(), (float)fb->Height() }, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});

			ImGui::EndChild();

			ImVec2 windowSize{ ImGui::GetWindowSize() };

			if (fb->Width() != windowSize.x || fb->Height() != windowSize.y)
			{
				fb->Resize(windowSize.x, windowSize.y);
			}
		}

		ImGui::End();
	}

	void SceneDisplay::Update()
	{
		if (!m_bPlayScene)
			return;

		auto& camera = m_Registry.GetContext<std::shared_ptr<SCION_RENDERING::Camera2D>>();

		if (!camera)
		{
			SCION_ERROR("Failed to get camera from registry context!");
		}

		camera->Update();

		auto& mainRegistry = SCION_CORE::ECS::MainRegistry::GetInstance();
		auto& coreEngine = SCION_CORE::CoreEngineData::GetInstance();

		auto& scriptSystem = m_Registry.GetContext<std::shared_ptr<SCION_CORE::Systems::ScriptingSystem>>();
		scriptSystem->Update();

		auto& pPhysicsWorld = m_Registry.GetContext<SCION_PHYSICS::PhysicalWorld>();
		pPhysicsWorld->Step(
			1.0f / 60.0f,
			10,
			8
		);
		pPhysicsWorld->ClearForces();

		auto& animationSystem = m_Registry.GetContext<std::shared_ptr<SCION_CORE::Systems::AnimationSystem>>();
		animationSystem->Update();
	}

}