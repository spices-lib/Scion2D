#include "PackageDisplay.h"
#include <Core/CoreUtilities/SaveProject.h>
#include <Core/CoreUtilities/CoreEngineData.h>
#include "editor/utilities/ImGuiUtils.h"
#include <imgui.h>
#include <Core/ECS/MainRegistry.h>
#include <filesystem>

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
		, bTitlebar{ false }
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

			ImGui::End();
		}

		ImGui::End();
	}

}