#include "ScriptDisplay.h"
#include <Core/ECS/MainRegistry.h>
#include <Logger.h>
#include <HelperUtilities.h>
#include <ScionUtilities.h>
#include <filesystem>
#include <fstream>
#include <Serializers/LuaSerializer.h>
#include <imgui.h>
#include <Utilities/DirectoryWatcher.h>

namespace fs = std::filesystem;
using namespace SCION_CORE;
using namespace SCION_FILESYSTEM;

namespace SCION_EDITOR {

	ScriptDisplay::ScriptDisplay()
		: m_sScriptsDirectory{ "" }
		, m_Selected{ -1 }
		, m_bScriptsChanged{ false }
		, m_bListScripts{ false }
		, m_pDirWatcher{ nullptr }
		, m_bFilesChanged{ false }
	{
		const std::string sScriptListPath = m_sScriptsDirectory + "script_list.lua";

		if (!fs::exists(fs::path{sScriptListPath}))
		{
			std::ofstream file{sScriptListPath};
			file.close();

			if (!fs::exists(fs::path{sScriptListPath}))
			{
				return;
			}
		}

		GenerateScriptList();

		m_pDirWatcher = std::make_unique<SCION_FILESYSTEM::DirectoryWatcher>(
			fs::path{ m_sScriptsDirectory }, [this](const fs::path& file, bool bModified) {
				OnFileChanged(file, bModified);
			}
		);
	}

	ScriptDisplay::~ScriptDisplay()
	{}

	void ScriptDisplay::Draw()
	{
		if (!ImGui::Begin("Script List"))
		{
			ImGui::End();
			return;
		}

		ImGui::SeparatorText("Script List");

		ImGui::Text("Drag to reorder scripts:");
		ImGui::Separator();

		ImGui::Columns(2, nullptr, false);

		ImGui::BeginChild("ListBox", ImVec2{ 0.0f, 300.0f }, true);
		for (int i = 0; i < (int)m_ScriptList.size(); i++)
		{
			const auto& script = m_ScriptList[i];
			if (ImGui::Selectable(script.c_str()), m_Selected == i)
			{
				m_Selected = i;
			}

			if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
			{
				int iNext = i + (ImGui::GetMouseDragDelta(0).y < 0.0f ? -1 : 1);
				if (iNext >= 0 && iNext < m_ScriptList.size())
				{
					std::swap(m_ScriptList.at(i), m_ScriptList.at(iNext));
					ImGui::ResetMouseDragDelta();
				}
			}
		}

		ImGui::EndChild();
		ImGui::NextColumn();

		ImGui::BeginChild("Controls", ImVec2{0.0f, 300.0f}, false, ImGuiWindowFlags_NoBackground);

		float buttonHeight = ImGui::GetFrameHeight();
		float totalHeight = buttonHeight * 2 * ImGui::GetStyle().ItemSpacing.y;

		float available = ImGui::GetContentRegionAvail().y;
		float padding = (available - totalHeight) * 0.5f;

		if (padding > 0.0f)
		{
			ImGui::Dummy(ImVec2{ 0.0f, padding });
		}

		if (ImGui::ArrowButton("##Up", ImGuiDir_Up))
		{
			if (m_Selected >= 0 && !m_ScriptList.empty())
			{
				int newIndex = (m_Selected - 1 + m_ScriptList.size()) % m_ScriptList.size();
				std::swap(m_ScriptList[m_Selected], m_ScriptList[newIndex]);
				m_Selected = newIndex;
			}
		}

		if (ImGui::ArrowButton("##Down", ImGuiDir_Down))
		{
			if (m_Selected >= 0 && !m_ScriptList.empty())
			{
				int newIndex = (m_Selected + 1) % m_ScriptList.size();
				std::swap(m_ScriptList[m_Selected], m_ScriptList[newIndex]);
				m_Selected = newIndex;
			}
		}

		ImGui::EndChild();

		ImGui::Columns(1);

		if (true)
		{
			if (ImGui::Button("Regenerate Script List"))
			{
				WriteScriptListToFile();
			}
		}

		ImGui::End();
	}

	void ScriptDisplay::Update()
	{
		if (m_bFilesChanged.exchange(false, std::memory_order_acquire))
		{
			m_bListScripts = true;
			SCION_LOG("File was changed or added to scripts directory.");
		}

		if (m_bListScripts)
		{
			m_Scripts.clear();
			for (const auto& dirEntity : fs::recursive_directory_iterator(fs::path{m_sScriptsDirectory}))
			{
				if (fs::is_directory(dirEntity) || dirEntity.path().extension() != ".lua" ||
					dirEntity.path().filename().string() == "main.lua" ||
					dirEntity.path().filename().string() == "script_list.lua")
				{
					continue;
				}

				auto foundScript = std::string{ (dirEntity.path().relative_path().string() + "scripts") };
				if (!foundScript.empty())
				{
					m_Scripts.push_back(foundScript);
				}
			}

			std::unordered_set<std::string> lookupSet(m_Scripts.begin(), m_Scripts.end());

			auto removeRange = std::ranges::remove_if(m_ScriptList, [&lookupSet](const std::string& item) { return !lookupSet.contains(item); });

			m_ScriptList.erase(removeRange.begin(), removeRange.end());

			for (const auto& script : m_Scripts)
			{
				if (std::ranges::find(m_ScriptList, script) == m_ScriptList.end())
				{
					m_ScriptList.push_back(script);
					m_bScriptsChanged = true;
				}
			}

			m_bListScripts = false;
		}
	}

	void ScriptDisplay::GenerateScriptList()
	{
		if (m_ScriptList.empty())
		{
			const std::string sScriptListPath = m_sScriptsDirectory + "script_list.lua";
			if (fs::exists(fs::path{sScriptListPath}))
			{
				sol::state lua{};
				auto result = lua.safe_script_file(sScriptListPath);
				if (!result.valid())
				{
					sol::error err = result;
					SCION_ERROR("Failed to load: {}", err.what());
					return;
				}

				sol::optional<sol::table> scriptList = lua["ScriptList"];
				if (!scriptList)
				{
					SCION_ERROR("Failed to load script list.");
					return;
				}

				std::string sPath{ m_sScriptsDirectory.substr(0, m_sScriptsDirectory.find("scripts")) };
				for (const auto& [_, script] : *scriptList)
				{
					std::string newScript{ script.as<std::string>() };
					if (fs::exists(fs::path{sPath + newScript}))
					{
						m_ScriptList.push_back(newScript);
					}
				}
			}
		}
	}

	void ScriptDisplay::WriteScriptListToFile()
	{
		const std::string sScriptListPath = m_sScriptsDirectory + "script_list.lua";

		if (!fs::exists(fs::path{sScriptListPath}))
		{
			SCION_ERROR("Failed to write script list.");
			return;
		}

		std::unique_ptr<LuaSerializer> pSerializer{ nullptr };

		try
		{
			pSerializer = std::make_unique<LuaSerializer>(sScriptListPath);
		}
		catch (const std::exception& ex)
		{
			SCION_ERROR("Failed to write script list, {}", ex.what());
			return;
		}

		pSerializer->AddComment();
		for (const auto& script : m_ScriptList)
		{
			pSerializer->AddValue();
		}

		pSerializer->Finish();
	}

	void ScriptDisplay::OnFileChanged(const std::filesystem::path& path, bool bModified)
	{
		m_bFilesChanged.store(true, std::memory_order_relaxed);
	}

}