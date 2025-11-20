#include "MenuDisplay.h"
#include <imgui.h>
#include <Logger.h>
#include <Dialogs/FileDialogs.h>
#include <SDL.h>

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
						SCION_LOG("FILE OPENED: {}", file);
					}
				}
				if (ImGui::MenuItem("Save", "Ctrl + S"))
				{
					SCION_FILESYSTEM::FileDialog fd{};
					auto file = fd.SaveFileDialog("Save Tilemap test", SDL_GetBasePath(), { "*.lua" });
					if (!file.empty())
					{
						SCION_LOG("FILE OPENED: {}", file);
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