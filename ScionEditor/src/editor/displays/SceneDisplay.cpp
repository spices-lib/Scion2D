#include "SceneDisplay.h"
#include <imgui.h>
#include <Rendering/Buffers/Framebuffer.h>

namespace SCION_EDITOR {

	SceneDisplay::SceneDisplay(SCION_CORE::ECS::Registry& registry)
		: m_Registry(registry)
	{
	}

	void SceneDisplay::Draw()
	{
		static bool pOpen{ true };
		if (!ImGui::Begin("Scene", &pOpen))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginChild("##SceneChild", ImVec2{ 0.0f, 0.0f }, NULL, ImGuiWindowFlags_NoScrollWithMouse))
		{
			const auto& fb = m_Registry.GetContext<std::shared_ptr<SCION_RENDERING::Framebuffer>>();
	
			ImGui::SetCursorPos(ImVec2{ 0.0f, 0.0f });

			ImGui::Image(fb->GetTextureID(), ImVec2{ (float)fb->Width(), (float)fb->Height() }, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});

			ImGui::EndChild();

			ImVec2 windowSize{ ImGui::GetWindowSize() };

			if (fb->Width() != windowSize.x || fb->Height() != windowSize.y)
			{
				fb->Resize(windowSize.x, windowSize.y);
			}
		}

		ImGui::End();
	}

}