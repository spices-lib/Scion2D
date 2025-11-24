#include "ScriptDisplay.h"
#include <Core/ECS/MainRegistry.h>
#include <Logger.h>
#include <HelperUtilities.h>
#include <ScionUtilities.h>
#include <filesystem>
#include <fstream>
#include <Serializers/LuaSerializer.h>

namespace fs = std::filesystem;
using namespace SCION_CORE;
using namespace SCION_FILESYSTEM;

namespace SCION_EDITOR {

	ScriptDisplay::ScriptDisplay()
		: m_sScriptsDirectory{ "" }
		, m_Selected{ -1 }
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
	}

	ScriptDisplay::~ScriptDisplay()
	{}

	void ScriptDisplay::Draw()
	{

	}

	void ScriptDisplay::Update()
	{

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
	}

}