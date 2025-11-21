#include "TileDetailsDisplay.h"
#include "editor/utilities/ImGuiUtils.h"
#include "editor/utilities/DrawComponentUtils.h"
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include "editor/tools/TileTool.h"
#include "editor/tools/ToolManager.h"
#include <Core/ECS/MainRegistry.h>
#include <Core/ECS/Components/ComponentSerializer.h>
#include <Core/Resources/AssetManager.h>
#include <Core/CoreUtilities/CoreUtilities.h>
#include <Logger.h>
#include <imgui.h>
#include <ranges>

using namespace SCION_CORE::ECS;

namespace SCION_EDITOR {

	void TileDetailsDisplay::DrawSpriteComponent(SCION_CORE::ECS::SpriteComponent& component, SceneObject* pScene)
	{
		bool bChanged{ false };

		ImGui::SeparatorText("sptite");
		ImGui::PushID(entt::type_hash<TransformComponent>::value());
		if (ImGui::TreeNodeEx("##SpriteTree", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::AddSpaces(2);

			ImVec4 col = { component.color.r / 255.0f, component.color.g / 255.0f, component.color.b / 255.0f, component.color.a / 255.0f };
			ImGui::InlineLabel("color");
			if (ImGui::ColorEdit4("##color", &col.x, IMGUI_COLOR_PICKER_FLAGS))
			{
				component.color.r = static_cast<GLubyte>(col.x * 255.0f);
				component.color.g = static_cast<GLubyte>(col.y * 255.0f);
				component.color.b = static_cast<GLubyte>(col.z * 255.0f);
				component.color.a = static_cast<GLubyte>(col.w * 255.0f);
			}

			ImGui::AddSpaces(2);

			ImGui::InlineLabel("texture: ");
			ImGui::TextColored(ImVec4{0.0f, 1.0f, 0.0f, 1.0f}, component.texture_name.c_str());

			std::string sLayer{ "" };

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

	TileDetailsDisplay::TileDetailsDisplay()
		: m_SelectedLayer{ -1 }
		, m_sRenameLayerBuf{ "" }
		, m_bRename{ false }
	{

	}

	TileDetailsDisplay::~TileDetailsDisplay()
	{

	}

	void TileDetailsDisplay::Draw()
	{
		auto pCurrentScene = SceneManager::GetInstance().GetCurrentScene();
		auto& toolManager = SceneManager::GetInstance().GetToolManager();

		auto pActiveTool = toolManager.GetActiveTool();
		if (!pActiveTool)
			return;

		if (!ImGui::Begin("Tile Details") || !pCurrentScene)
		{
			ImGui::End();
			return;
		}

		auto& tileData = pActiveTool->GetTileData();

		DrawComponentsUtil::DrawComponentInfo(tileData.transform);

		ImGui::AddSpaces(2);

		DrawSpriteComponent(tileData.sprite, pCurrentScene.get());

		ImGui::AddSpaces(2);
		ImGui::Separator();
		ImGui::AddSpaces(2);

		ImGui::Checkbox("Box Collider", &tileData.bColloder);
		if (tileData.bColloder)
		{
			DrawComponentsUtil::DrawComponentInfo(tileData.boxCollider);
		}

		ImGui::AddSpaces(2);
		ImGui::Separator();
		ImGui::AddSpaces(2);

		ImGui::Checkbox("Circle Collider", &tileData.bCircle);
		if (tileData.bCircle)
		{
			DrawComponentsUtil::DrawComponentInfo(tileData.circleCollider);
		}

		ImGui::AddSpaces(2);
		ImGui::Separator();
		ImGui::AddSpaces(2);

		ImGui::Checkbox("Animation", &tileData.bAnimation);
		if (tileData.bAnimation)
		{
			DrawComponentsUtil::DrawComponentInfo(tileData.animation);
		}

		ImGui::AddSpaces(2);
		ImGui::Separator();
		ImGui::AddSpaces(2);

		ImGui::Checkbox("Physics", &tileData.bPhysics);
		if (tileData.bPhysics)
		{
			DrawComponentsUtil::DrawComponentInfo(tileData.physics);
		}

		ImGui::AddSpaces(2);
		ImGui::Separator();
		ImGui::AddSpaces(2);

		if (ImGui::BeginChild(" Tile Layers"))
		{
			ImGui::SeparatorText("Tile Layers");
			auto& spriteLayers = pCurrentScene->GetLayerParams();

			if (ImGui::Button("Add"))
			{
				pCurrentScene->AddNewLayer();
			}

			ImGui::AddSpaces(2);
			ImGui::Separator();
			ImGui::AddSpaces(2);

			float itemWidth{ ImGui::GetWindowWidth() - 32.0f };
			auto rView = spriteLayers | std::ranges::views::reverse;

			for (auto it = rView.begin(); it != rView.end(); it++)
			{
				int n = std::distance(std::begin(rView), it);

				auto& spriteLayer = *it;
				bool bIsSelected{ m_SelectedLayer == n };

				ImGui::Selectable(spriteLayer.sLayerName.c_str(), false, 0, ImVec2{ itemWidth, 20.0f });

				if (ImGui::IsItemActivated() && !ImGui::IsItemHovered())
				{
					int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.0f ? -1 : 1);
					if (n_next >= 0 && n_next < rView.size())
					{
						std::swap(rView[n], rView[n_next]);

						auto spriteView = pCurrentScene->GetRegistry().GetRegistry().view<SpriteComponent, TileComponent>();

						int invNext = std::abs(n_next - static_cast<int>(rView.size() - 1));
						int invCurrent = std::abs(n - static_cast<int>(rView.size() - 1));

						for (auto entity : spriteView)
						{
							auto& sprite = spriteView.get<SpriteComponent>(entity);
							if (sprite.layer == invCurrent)
							{
								sprite.layer = invNext;
							}
							else if (sprite.layer == invNext)
							{
								sprite.layer = invCurrent;
							}
						}

						m_SelectedLayer = n_next;
						tileData.sprite.layer = invNext;

						ImGui::ResetMouseDragDelta();
					}
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0) && !m_bRename)
				{
					m_SelectedLayer = n;
					tileData.sprite.layer = std::abs(m_SelectedLayer - static_cast<int>(rView.size() - 1));
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && !m_bRename && bIsSelected)
				{
					m_bRename = true;
					m_sRenameLayerBuf.clear();
					m_sRenameLayerBuf = spriteLayer.sLayerName;
				}

				if (m_bRename && bIsSelected)
				{

				}

				ImGui::SameLine();

				ImGui::Checkbox(spriteLayer.sLayerName.c_str(), &spriteLayer.bVisible);
			}

			ImGui::EndChild();
		}

		ImGui::End();
	}

}