#include "DrawComponentUtils.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/Resources/AssetManager.h"
#include "Core/CoreUtilities/CoreUtilities.h"
#include "ScionUtilities.h"
#include <Logger.h>
#include "ImGuiUtils.h"

using namespace SCION_CORE::ECS;

namespace SCION_EDITOR {

	void DrawComponentsUtil::DrawImGuiComponent(SCION_CORE::ECS::TransformComponent& component)
	{
		ImGui::SeparatorText("Transform");
		ImGui::PushID(entt::type_hash<TransformComponent>::value());
		if (ImGui::TreeNodeEx("##TransformTree", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushItemWidth(120.0f);
			ImGui::InlineLabel("position");
			ImGui::ColoredLabel("x", LABEL_SINGLE_SIZE, LABEL_RED);
			ImGui::SameLine();
			ImGui::InputFloat("##position_x", &component.position.x, 1.0f, 10.0f, "%.1f");
			ImGui::SameLine();
			ImGui::ColoredLabel("y", LABEL_SINGLE_SIZE, LABEL_GREEN);
			ImGui::SameLine();
			ImGui::InputFloat("##position_y", &component.position.y, 1.0f, 10.0f, "%.1f");

			ImGui::InlineLabel("scale");
			ImGui::ColoredLabel("x", LABEL_SINGLE_SIZE, LABEL_RED);
			ImGui::SameLine();
			ImGui::InputFloat("##scale_x", &component.scale.x, 1.0f, 10.0f, "%.1f");
			ImGui::SameLine();
			ImGui::ColoredLabel("y", LABEL_SINGLE_SIZE, LABEL_GREEN);
			ImGui::SameLine();
			ImGui::InputFloat("##scale_y", &component.scale.y, 1.0f, 10.0f, "%.1f");

			ImGui::InlineLabel("rotation");
			ImGui::InputFloat("##rotation", &component.rotation, 1.0f, 1.0f, "%.1f");
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void DrawComponentsUtil::DrawImGuiComponent(SCION_CORE::ECS::SpriteComponent& component)
	{
		bool bChanged{ false };

		ImGui::SeparatorText("sptite");
		ImGui::PushID(entt::type_hash<TransformComponent>::value());
		if (ImGui::TreeNodeEx("##SpriteTree", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::AddSpaces(2);
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetDisplayTextures");

				if (payload)
				{
					const char* texture = (const char*)payload->Data;
					std::string textureStr{ texture };
					if (!textureStr.empty())
					{
						component.texture_name = textureStr;
					}
				}

				ImGui::EndDragDropSource();
			}

			ImVec4 col = { component.color.r / 255.0f, component.color.g / 255.0f, component.color.b / 255.0f, component.color.a / 255.0f };
			ImGui::InlineLabel("color");
			if (ImGui::ColorEdit4("##color", &col.x, IMGUI_COLOR_PICKER_FLAGS))
			{
				component.color.r = static_cast<GLubyte>(col.x * 255.0f);
				component.color.g = static_cast<GLubyte>(col.y * 255.0f);
				component.color.b = static_cast<GLubyte>(col.z * 255.0f);
				component.color.a = static_cast<GLubyte>(col.w * 255.0f);
			}

			auto& assetManager = MainRegistry::GetInstance().GetAssetManager();

			std::string sSelectedTexture{ component.texture_name };
			ImGui::InlineLabel("texture");
			if (ImGui::BeginCombo("##texture", sSelectedTexture.c_str()))
			{
				for (const auto& sTextureName : assetManager.GetAssetKeyNames(SCION_UTL::AssetType::TEXTURE))
				{
					if (ImGui::Selectable(sTextureName.c_str(), sTextureName == sSelectedTexture))
					{
						sSelectedTexture = sTextureName;
						component.texture_name = sSelectedTexture;
						bChanged = true;
					}
				}
				ImGui::EndCombo();
			}

			ImGui::AddSpaces(2);

			ImGui::PushItemWidth(120.0f);
			ImGui::InlineLabel("width");
			if (ImGui::InputFloat("##width", &component.width, 8.0f, 8.0f))
			{
				component.width = std::clamp(component.width, 8.0f, 1366.0f);
				bChanged = true;
			}

			ImGui::InlineLabel("height");
			if (ImGui::InputFloat("##height", &component.height, 8.0f, 8.0f))
			{
				component.height = std::clamp(component.height, 8.0f, 768.0f);
				bChanged = true;
			}

			ImGui::InlineLabel("layer");
			if (ImGui::InputInt("##layer", &component.layer, 0, 10))
			{
				component.layer = std::clamp(component.layer, 0, 10);
				bChanged = true;
			}

			ImGui::InlineLabel("Sprite Sheet Position");
			ImGui::ColoredLabel("x", LABEL_SINGLE_SIZE, LABEL_RED);
			ImGui::SameLine();
			if (ImGui::InputInt("##start_x", &component.start_x, 1, 1))
			{
				component.start_x = std::clamp(component.start_x, 0, 32);
				bChanged = true;
			}
			ImGui::SameLine();
			ImGui::ColoredLabel("y", LABEL_SINGLE_SIZE, LABEL_RED);
			ImGui::SameLine();
			if (ImGui::InputInt("##start_y", &component.start_y, 1, 1))
			{
				component.start_y = std::clamp(component.start_y, 0, 32);
				bChanged = true;
			}

			ImGui::TreePop();
			ImGui::PopItemWidth();
		}

		ImGui::PopID();

		if (bChanged)
		{
			auto pTexture = MainRegistry::GetInstance().GetAssetManager().GetTexture(component.texture_name);
			
			SCION_CORE::GenerateUVs(component, pTexture.GetWidth(), pTexture.GetHeight());
		}
	}

	void DrawComponentsUtil::DrawImGuiComponent(SCION_CORE::ECS::BoxColliderComponent& component)
	{
	}
	void DrawComponentsUtil::DrawImGuiComponent(SCION_CORE::ECS::CircleColliderComponent& component)
	{
	}
	void DrawComponentsUtil::DrawImGuiComponent(SCION_CORE::ECS::PhysicsComponent& component)
	{
	}
	void DrawComponentsUtil::DrawImGuiComponent(SCION_CORE::ECS::TextComponent& component)
	{
	}
	void DrawComponentsUtil::DrawImGuiComponent(SCION_CORE::ECS::AnimationComponent& component)
	{
	}
	void DrawComponentsUtil::DrawImGuiComponent(SCION_CORE::ECS::RigidBodyComponent& component)
	{
	}

}