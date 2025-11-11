#pragma once
#include <Physics/Box2DWrappers.h>
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <entt.hpp>

namespace SCION_CORE::ECS {

	constexpr float METERS_TO_PIXELS = 12.f;
	constexpr float PIXELS_TO_METERS = 1.f / METERS_TO_PIXELS;

	enum class RigidBodyType { STATIC = 0, KINEMATIC, DYNAMIC };

	struct PhysicsAttributes
	{
		RigidBodyType eType{ RigidBodyType::STATIC };
		float density{ 1.0f }, friction{ 0.2f }, restitution{ 0.2f };
		float restitutionThreshold{ 1.0f }, radius{ 0.0f }, gravityScale{ 1.0f };

		glm::vec2 position{ 0.0f}, scale{ 1.0f }, boxSize{ 0.0f }, offset{ 0.0f };
		bool bCircle{ false }, bBoxShape{ true }, bFixdRotation{ true };
		uint16_t filterCategory{ 0 }, filterMask{ 0 };
		int16_t groupIndex{ 0 };
	};

	struct PhysicsComponent
	{
		SCION_PHYSICS::PhysicalWorld m_pPhysicsWorld;
		std::shared_ptr<b2Body> m_pRigidBody;
		PhysicsAttributes m_InitialAttribs;

	public:

		PhysicsComponent() = default;
		PhysicsComponent(SCION_PHYSICS::PhysicalWorld pPhysicsWorld, const PhysicsAttributes& physicsAttr);
		~PhysicsComponent() = default;

		void Init(int windowWidth, int windowHeight);
		b2Body* GetBody() { return m_pRigidBody.get(); }

		static void CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry);
	};
}
