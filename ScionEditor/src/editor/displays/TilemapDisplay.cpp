#include "TilemapDisplay.h"
#include <Rendering/Core/Camera.h>
#include "editor/systems/GridSystem.h"
#include "editor/utilities/EditorFramebuffer.h"
#include <Rendering/Core/Renderer.h>
#include <Logger.h>
#include <imgui.h>
#include <Core/ECS/MainRegistry.h>

namespace SCION_EDITOR {

	void TilemapDisplay::RenderTilemap()
	{
		auto& mainRegistry = SCION_CORE::ECS::MainRegistry::GetInstance();
		auto editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<SCION_EDITOR::EditorFramebuffers>>();
		auto renderer = mainRegistry.GetContext<std::shared_ptr<SCION_RENDERING::Renderer>>();
		const auto& fb = editorFramebuffers->mapFramebuffer[FramebufferType::TILEMAP];

		fb->Bind();
		renderer->SetViewport(0, 0, fb->Width(), fb->Height());
		renderer->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderer->ClearBuffers(true, true, true);

		auto gridSystem = mainRegistry.GetContext<std::shared_ptr<GridSystem>>();
		gridSystem->Update(*m_pTilemapCam);

		fb->Unbind();
		fb->CheckResize();

		renderer->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderer->ClearBuffers(true, true, true);
	}

	TilemapDisplay::TilemapDisplay()
		: m_pTilemapCam{ std::make_unique<SCION_RENDERING::Camera2D>() }
	{

	}

	void TilemapDisplay::Draw()
	{
		if (!ImGui::Begin("Tilemap Editor"))
		{
			ImGui::End();
			return;
		}

		RenderTilemap();

		auto& mainRegistry = SCION_CORE::ECS::MainRegistry::GetInstance();

		if (ImGui::BeginChild("##tilemap", ImVec2{ 0, 0 }, false, ImGuiWindowFlags_NoScrollWithMouse))
		{
			auto editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<SCION_EDITOR::EditorFramebuffers>>();
			const auto& fb = editorFramebuffers->mapFramebuffer[FramebufferType::TILEMAP];

			ImVec2 imageSize{ static_cast<float>(fb->Width()), static_cast<float>(fb->Height()) };
			ImVec2 windowSize{ ImGui::GetWindowSize() };

			float x = (windowSize.x - imageSize.x) * 0.5f;
			float y = (windowSize.y - imageSize.y) * 0.5f;

			ImGui::SetCursorPos(ImVec2{ x, y });

			ImGui::Image(fb->GetTextureID(), imageSize, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

			ImGui::EndChild();
		}

		ImGui::End();
	}

	void TilemapDisplay::Update()
	{
		
	}

}