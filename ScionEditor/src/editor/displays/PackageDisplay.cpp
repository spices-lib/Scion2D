#include "PackageDisplay.h"
#include <Core/CoreUtilities/SaveProject.h>
#include <Core/CoreUtilities/CoreEngineData.h>
#include "editor/utilities/ImGuiUtils.h"
#include <imgui.h>
#include <Core/ECS/MainRegistry.h>
#include <filesystem>
#include "editor/scene/SceneManager.h"
#include <Logger.h>
#include "editor/displays/PackageDisplay.h"

using namespace SCION_CORE::ECS;
namespace fs = std::filesystem;

namespace SCION_EDITOR {

	PackageGameDisplay::PackageGameDisplay()
		: m_pGameConfig{ std::make_unique<SCION_CORE::GameConfig>() }
		, m_sDestinationPath{ "" }
		, m_sScriptListPath{ "" }
		, m_sFileIconPath{ "" }
		, m_bResizable{ false }
		, m_bBorderless{ false }
		, m_bFullScreen{ false }
		, m_bTitlebar{ false }
		, m_bScriptListExists{ false }
	{
		const auto& pSaveProject = MainRegistry::GetInstance().GetContext<std::shared_ptr<SCION_CORE::SaveProject>>();
		m_sScriptListPath = "";

		m_bScriptListExists = fs::exists(fs::path{m_sScriptListPath});
	}

	PackageGameDisplay::~PackageGameDisplay()
	{}

	void PackageGameDisplay::Update()
	{

	}

	void PackageGameDisplay::Draw()
	{
		ImGui::Begin("Package Game");

		ImGui::SeparatorText("Package and Export Game.");
		ImGui::NewLine();

		if (ImGui::BeginChild("Game Packager"))
		{
			ImGui::SeparatorText("File Information");
			ImGui::PushItemWidth(256.0F);
			ImGui::InlineLabel("Game Title");
			ImGui::InputText("##gameTitle", m_pGameConfig->sGameName);

			ImGui::InlineLabel("Destination");
			ImGui::InputText("##destination", m_sDestinationPath);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("...""##dest"))
			{

			}

			ImGui::InlineLabel("Icon");
			ImGui::PushItemWidth(256.0f);
			ImGui::InputText("##icon", m_sDestinationPath);
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("...""##iconpath"))
			{

			}

			ImGui::InlineLabel("Package Assets");
			ImGui::SetItemTooltip("Convert assets into luac files.");
			ImGui::Checkbox("##packageassets", &m_pGameConfig->bPackageAssets);
			ImGui::AddSpaces(2);
			ImGui::Separator();
			ImGui::AddSpaces(3);

			ImGui::SeparatorText("Default Window Parameters");
			ImGui::AddSpaces(2);
			ImGui::InlineLabel("Window Size");
			ImGui::PushItemWidth(128.0f);
			ImGui::InputInt("##windowWidth", &m_pGameConfig->windowWidth);
			ImGui::SameLine();
			ImGui::InputInt("##windowHeight", &m_pGameConfig->windowHeight);
			ImGui::PopItemWidth();
			ImGui::Separator();
			ImGui::AddSpaces(2);
			ImGui::Text("Window Flags");
			ImGui::Separator();

			ImGui::InlineLabel("Resizable");
			if (ImGui::Checkbox("##resizeable", &m_bResizable))
			{
				if (m_bResizable)
				{
					m_bFullScreen = false;
				}
			}

			ImGui::InlineLabel("Fullscreen");
			if (ImGui::Checkbox("##fullscreen", &m_bResizable))
			{
				if (m_bFullScreen)
				{
					m_bResizable = false;
					m_bTitlebar = false;
				}
			}

			ImGui::InlineLabel("Titlebar");
			if (ImGui::Checkbox("##titlebar", &m_bResizable))
			{
				if (m_bTitlebar)
				{
					m_bFullScreen = false;
					m_bResizable = false;
				}
			}

			ImGui::InlineLabel("Borderless");
			if (ImGui::Checkbox("##borderless", &m_bResizable))
			{
				if (m_bBorderless)
				{
					m_bTitlebar = false;
				}
			}

			ImGui::Separator();

			ImGui::AddSpaces(3);

			ImGui::SeparatorText("Default Camera Parameters");

			ImGui::AddSpaces(3);

			ImGui::InlineLabel("Camera Size");
			ImGui::PushItemWidth(128.0f);
			ImGui::InputInt("##cameraWidth", &m_pGameConfig->cameraWidth);
			ImGui::SameLine();
			ImGui::InputInt("##cameraHeight", &m_pGameConfig->cameraHeight);
			ImGui::PopItemWidth();

			ImGui::InlineLabel("Camera Scale");
			ImGui::InputFloat("##camerascale", &m_pGameConfig->cameraScale);

			ImGui::AddSpaces(3);

			ImGui::SeparatorText("Startup Options");

			ImGui::PushItemWidth(256.0f);
			if (ImGui::BeginCombo("##start_up_scenes", m_pGameConfig->sStartupScene.c_str()))
			{
				for (const auto& sSceneName : SceneManager::GetInstance().GetSceneNames())
				{
					if (ImGui::Selectable(sSceneName.c_str(), sSceneName == m_pGameConfig->sStartupScene))
					{
						m_pGameConfig->sStartupScene = sSceneName;
					}
				}

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::End();
		}

		ImGui::Separator();

		ImGui::AddSpaces(3);

		ImGui::End();
	}

}