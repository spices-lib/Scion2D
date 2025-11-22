#include "SceneHierarchyDisplay.h"
#include "editor/scene/SceneManager.h"
#include "editor/scene/SceneObject.h"
#include <Core/ECS/MainRegistry.h>
#include <Core/ECS/Entity.h>
#include <imgui.h>
#include <Core/ECS/Components/ComponentSerializer.h>
#include <Core/ECS/MetaUtilities.h>
#include "editor/utilities/DrawComponentUtils.h"

using namespace SCION_CORE::ECS;

namespace SCION_EDITOR {

	auto create_entity = [&](SCION_EDITOR::SceneObject& currentScene) {
		Entity newEntity{ currentScene.GetRegistry(), "GameObject", "" };
		newEntity.AddComponent<TransformComponent>();
	};

	bool SceneHierarchyDisplay::OpenTreeNode(SCION_CORE::ECS::Entity& entity)
	{
		const auto& name = entity.GetName();

		ImGui::PushID(static_cast<int32_t>(entity.GetEntity()));

		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding |
			ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (m_pSelectedEntity && m_pSelectedEntity->GetEntity() == entity.GetEntity())
		{
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}

		bool bTreeNodeOpen{ false };
		bTreeNodeOpen = ImGui::TreeNodeEx(name.c_str(), nodeFlags);

		if (ImGui::IsItemClicked())
		{
			m_pSelectedEntity = std::make_shared<Entity>(SceneManager::GetInstance().GetCurrentScene()->GetRegistry(), entity.GetEntity());
		}

		ImGui::PopID();

		return bTreeNodeOpen;
	}

	void SceneHierarchyDisplay::AddComponent(SCION_CORE::ECS::Entity& entity, bool* bAddComponent)
	{
		if (!ImGui::Begin("Object Details"))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Add Component"))
				m_bAddedComponent = true;

			if (m_pSelectedEntity && m_bAddedComponent)
				AddComponent(*m_pSelectedEntity, &m_bAddedComponent);
		}
	}

	void SceneHierarchyDisplay::DrawGameObjectDetails()
	{

	}

	void SceneHierarchyDisplay::DrawEntityComponents()
	{
		if (!m_pSelectedEntity)
			return;

		auto& registry = m_pSelectedEntity->GetRegistry();
		for (const auto&& [id, storage] : registry.storage())
		{
			if (!storage.contains(m_pSelectedEntity->GetEntity()))
				continue;

			if (id == entt::type_hash<TileComponent>::value())
				continue;

			//const auto drawInfo = SCION_CORE::Utils::InvokeMetaFunction(id, "DrawEntityComponentInfo"_hs, *m_pSelectedEntity);

			//if (drawInfo)
			{
				ImGui::Spacing();
				ImGui::PushID(id);
				if (ImGui::Button("remove"))
				{
					storage.remove(m_pSelectedEntity->GetEntity());
				}
				ImGui::PopID();
			}

			ImGui::Spacing();
			ImGui::Separator();
		}
	}

	SceneHierarchyDisplay::SceneHierarchyDisplay()
	{

	}

	SceneHierarchyDisplay::~SceneHierarchyDisplay()
	{

	}

	void SceneHierarchyDisplay::Update()
	{

	}

	void SceneHierarchyDisplay::Draw()
	{
		auto pCurrentScnen = SceneManager::GetInstance().GetCurrentScene();
		if (!ImGui::Begin("Scene Hierarchy") || !pCurrentScnen)
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Add New Game object"))
				create_entity(*pCurrentScnen);

			ImGui::EndPopup();
		}

		auto& registry = pCurrentScnen->GetRegistry();
		auto sceneEntities = registry.GetRegistry().view<entt::entity>(entt::exclude<TileComponent, ScriptComponent>);

		for (auto entity : sceneEntities)
		{
			Entity ent{ registry, entity };
			if (OpenTreeNode(ent))
			{
				ImGui::TreePop();
			}
		}

		ImGui::End();

		DrawGameObjectDetails();
	}

}