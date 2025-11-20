#include "MenuDisplay.h"
#include <imgui.h>
#include <Logger.h>
#include <Dialogs/FileDialogs.h>
#include <SDL.h>
#include <Core/Loaders/TilemapLoader.h>
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"

namespace SCION_EDITOR {

	void MenuDisplay::Draw()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl + N"))
				{

				}
				if (ImGui::MenuItem("Open", "Ctrl + O"))
				{
					SCION_FILESYSTEM::FileDialog fd{};
					auto file = fd.OpenFileDialog("Open tests", SDL_GetBasePath(), { "*.png", "*.jpg"});
					if (!file.empty())
					{
						auto pCurrentScene = SceneManager::GetInstance().GetCurrentScene();
						if (pCurrentScene)
						{
							SCION_CORE::Loaders::TilemapLoader tl{};
							if (!tl.SaveTilemap(pCurrentScene->GetRegistry(), file, true))
							{
								SCION_ERROR("Failed to load tilemap.")
							}
						}
						else
						{
							SCION_ERROR("Failed to load tilemap.")
						}
					}
				}
				if (ImGui::MenuItem("Save", "Ctrl + S"))
				{
					SCION_FILESYSTEM::FileDialog fd{};
					auto file = fd.SaveFileDialog("Save Tilemap test", SDL_GetBasePath(), { "*.lua" });
					if (!file.empty())
					{
						auto pCurrentScene = SceneManager::GetInstance().GetCurrentScene();
						if (pCurrentScene)
						{
							SCION_CORE::Loaders::TilemapLoader tl{};
							if (!tl.SaveTilemap(pCurrentScene->GetRegistry(), file, true))
							{
								SCION_ERROR("Failed to save tilemap.");
								return;
							}
						}
						else
						{
							SCION_ERROR("Failed to save tilemap. No active scene");
							return;
						}
					}
				}
				if (ImGui::MenuItem("Exit"))
				{

				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

}