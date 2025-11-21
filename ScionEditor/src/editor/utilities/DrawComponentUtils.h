#pragma once
#include <Core/ECS/Components/ComponentSerializer.h>

namespace SCION_CORE::ECS {
	class Entity;
}

namespace SCION_EDITOR {

	class DrawComponentsUtil
	{
	public:

		DrawComponentsUtil() = delete;

		template<typename TComponent>
		static void DrawEntityComponentInfo(SCION_CORE::ECS::Entity& entity);

		template<typename TComponent>
		static void DrawComponentInfo(TComponent& component);

		template<typename TComponent>
		static void RegisterUIComponent();

	private:

		static void DrawImGuiComponent(SCION_CORE::ECS::TransformComponent& component);
		static void DrawImGuiComponent(SCION_CORE::ECS::SpriteComponent& component);
		static void DrawImGuiComponent(SCION_CORE::ECS::BoxColliderComponent& component);
		static void DrawImGuiComponent(SCION_CORE::ECS::CircleColliderComponent& component);
		static void DrawImGuiComponent(SCION_CORE::ECS::PhysicsComponent& component);
		static void DrawImGuiComponent(SCION_CORE::ECS::TextComponent& component);
		static void DrawImGuiComponent(SCION_CORE::ECS::AnimationComponent& component);
		static void DrawImGuiComponent(SCION_CORE::ECS::RigidBodyComponent& component);
	};

}

#include "DrawComponentUtils.inl"