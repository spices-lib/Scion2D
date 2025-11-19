#include "MenuDisplay.h"
#include <imgui.h>
#include <Logger.h>

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

				}
				if (ImGui::MenuItem("Save", "Ctrl + S"))
				{

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