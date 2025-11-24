#pragma once
#include "IDisplay.h"
#include <string>

namespace SCION_EDITOR {

	class ScriptDisplay : public IDisplay
	{
	public:

		ScriptDisplay();
		~ScriptDisplay() override;

		void Draw() override;
		void Update() override;

	private:

		void GenerateScriptList();
		void WriteScriptListToFile();

	private:

		std::vector<std::string> m_ScriptList;
		std::vector<std::string> m_Scripts;
		std::string m_sScriptsDirectory;
		int m_Selected;
	};
}