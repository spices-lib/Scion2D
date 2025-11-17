#include "TilemapDisplay.h"
#include <Rendering/Core/Camera.h>
#include "editor/systems/GridSystem.h"
#include "editor/utilities/EditorFramebuffer.h"
#include <Rendering/Core/Renderer.h>
#include <Logger.h>
#include <imgui.h>
#include <Core/ECS/MainRegistry.h>
#include "editor/utilities/EditorUtilities.h"
#include "editor/scene/SceneManager.h"
#include "editor/tools/ToolManager.h"
#include "editor/tools/CreateTileTool.h"
#include "Core/Systems/RenderSystem.h"
#include "Core/Systems/RenderShapeSystem.h"
#include "Core/Systems/RenderUISystem.h"
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include "editor/tools/TileTool.h"

using namespace SCION_CORE::Systems;

namespace SCION_EDITOR {

	void TilemapDisplay::RenderTilemap()
	{
		auto pCurrentScene = SceneManager::GetInstance().GetCurrentScene();
		auto& mainRegistry = SCION_CORE::ECS::MainRegistry::GetInstance();
		auto editorFramebuffers = mainRegistry.GetContext<std::shared_ptr<SCION_EDITOR::EditorFramebuffers>>();
		auto renderer = mainRegistry.GetContext<std::shared_ptr<SCION_RENDERING::Renderer>>();
		const auto& fb = editorFramebuffers->mapFramebuffer[FramebufferType::TILEMAP];

		auto renderSystem = mainRegistry.GetContext<std::shared_ptr<RenderSystem>>();
		auto renderShapeSystem = mainRegistry.GetContext<std::shared_ptr<RenderShapeSystem>>();
		auto renderUISystem = mainRegistry.GetContext<std::shared_ptr<RenderUISystem>>();

		fb->Bind();
		renderer->SetViewport(0, 0, fb->Width(), fb->Height());
		renderer->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderer->ClearBuffers(true, true, true);

		if (!pCurrentScene)
		{
			fb->Unbind();
			return;
		}

		auto gridSystem = mainRegistry.GetContext<std::shared_ptr<GridSystem>>();
		gridSystem->Update(*pCurrentScene, *m_pTilemapCam);

		renderSystem->Update();
		renderShapeSystem->Update();
		renderUISystem->Update(pCurrentScene->GetRegistry().GetRegistry());

		auto pActivateTool = SceneManager::GetInstance().GetToolManager().GetActiveTool();
		if (pActivateTool)
		{
			pActivateTool->Draw();
		}

		fb->Unbind();
		fb->CheckResize();

		renderer->SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderer->ClearBuffers(true, true, true);
	}

	void TilemapDisplay::LoadNewScene()
	{
		auto pCurrentScene = SceneManager::GetInstance().GetCurrentScene();

		if (!pCurrentScene)
			return;

		auto pActiveTool = SceneManager::GetInstance().GetToolManager().GetActiveTool();
		if (pActiveTool)
		{
			if (!pActiveTool->SetupTool(pCurrentScene->GetRegistryPtr(), m_pTilemapCam.get()))
			{

			}
			if (!SceneManager::GetInstance().GetCurrentTileset().empty())
			{
				pActiveTool->LoadSpriteTextureData(SceneManager::GetInstance().GetCurrentTileset());
			}
		}
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
			ImGuiIO io = ImGui::GetIO();

			auto relativePos = ImGui::GetCursorScreenPos();
			auto windowPos = ImGui::GetWindowPos();

			auto pActiveTool = SceneManager::GetInstance().GetToolManager().GetActiveTool();
			if (pActiveTool)
			{
				pActiveTool->SetRelativeCoords(glm::vec2{relativePos.x, relativePos.y});
				pActiveTool->SetCursorCoords(glm::vec2{io.MousePos.x, io.MousePos.y});
				pActiveTool->SetWindowPos(glm::vec2{windowPos.x, windowPos.y});
				pActiveTool->SetWindowSize(glm::vec2{windowSize.x, windowSize.y});
			}

			ImGui::Image(fb->GetTextureID(), imageSize, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DROP_SCENE_SRC);
				if (payload)
				{
					SceneManager::GetInstance().SetCurrentScene(std::string{ (const char*)payload->Data });
					LoadNewScene();
				}

				ImGui::EndDragDropTarget();
			}

			ImGui::EndChild();
		}

		ImGui::End();
	}

	void TilemapDisplay::Update()
	{
		auto pCurrentScene = SceneManager::GetInstance().GetCurrentScene();

		if (!pCurrentScene)
			return;

		auto pActiveTool = SceneManager::GetInstance().GetToolManager().GetActiveTool();

		if (pActiveTool && ImGui::GetDragDropPayload())
		{
			pActiveTool->Update(pCurrentScene->GetCanvas());
			pActiveTool->Create();
		}

		m_pTilemapCam->Update();
	}

}