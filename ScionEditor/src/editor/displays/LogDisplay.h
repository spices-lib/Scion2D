#pragma once
#include "IDisplay.h"
#include <imgui.h>
#include <string>

namespace SCION_EDITOR {

	class LogDisplay : public IDisplay
	{
	private:

		ImGuiTextBuffer m_TextBuffer;
		ImVector<int> m_TextOffsets;
		bool m_bAutoScroll;

	private:

		void GetLogs();

	public:

		LogDisplay();
		~LogDisplay() = default;

		void Clear();
		virtual void Draw() override;

	private:

		std::string m_sSearchQuery;
		bool m_bShowInfo;
		bool m_bShowWarn;
		bool m_bShowError;
		int m_LogIndex;

	};
}