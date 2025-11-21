#include "ImGuiUtils.h"

void ImGui::ColoredLabel(const std::string& label, const ImVec2& size, const ImVec4& color)
{
	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
	ImGui::Button(label.c_str(), size);
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void ImGui::OffsetTextX(const std::string& label, float position)
{
	ImGui::SetCursorPosX(position);
	ImGui::Text(label.c_str());
}

void ImGui::AddSpaces(int numSpaces)
{
	for (int i = 0; i < numSpaces; i++)
	{
		ImGui::Spacing();
	}
}

void ImGui::InlineLabel(const std::string& label, float spaceSize)
{
	ImGui::Text(label.c_str());
	ImGui::SameLine();
	ImGui::SetCursorPosX(spaceSize);
}
