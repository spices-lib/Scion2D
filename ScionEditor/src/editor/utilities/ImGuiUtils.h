#pragma once
#include <imgui.h>
#include <string>

constexpr ImVec4 LABEL_RED = ImVec4{ 0.92, 0.18, 0.05, 1.0 };
constexpr ImVec4 LABEL_GREEN = ImVec4{ 0.05, 0.93, 0.25, 1.0 };
constexpr ImVec4 LABEL_BLUE = ImVec4{ 0.05, 0.18, 0.91, 1.0 };

constexpr ImVec4 BUTTON_HELD = ImVec4{ 0.0f, 0.9f, 0.0f, 0.3f };

constexpr ImVec4 BLACK_TRANSPARENT = ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };

constexpr ImVec2 LABEL_SINGLE_SIZE = ImVec2{ 20.0, 20.0 };
constexpr ImVec2 TOOL_BUTTON_SIZE = ImVec2{ 32.0f, 32.0f };

constexpr ImGuiColorEditFlags IMGUI_COLOR_PICKER_FLAGS =
	ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB;

constexpr ImGuiTableFlags IMGUI_NORMAL_TABLE_FLAGS =
	ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;

namespace ImGui {

	void ColoredLabel(const std::string& label, const ImVec2& size = ImVec2{32.0f, 32.0f}, 
		const ImVec4& color = ImVec4{1.0f, 1.0f, 1.0f, 1.0f});
	void OffsetTextX(const std::string& label, float position);
	void AddSpaces(int numSpaces);
	void InlineLabel(const std::string& label, float spaceSize = 128.0f);

	void ActiveButton(const char* label, ImVec2 size = TOOL_BUTTON_SIZE);
	void DisabledButton(const char* label, ImVec2 size = TOOL_BUTTON_SIZE, const std::string& disabledMsg = "");
}