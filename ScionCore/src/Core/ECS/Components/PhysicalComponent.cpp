#include "PhysicalComponent.h"
#include <Logger.h>

namespace SCION_CORE::ECS {

	PhysicsComponent::PhysicsComponent(SCION_PHYSICS::PhysicalWorld pPhysicsWorld, const PhysicsAttributes& physicsAttr)
		: m_pPhysicsWorld{ pPhysicsWorld }, m_pRigidBody{ nullptr }, m_InitialAttribs{ physicsAttr }
	{

	}

	void PhysicsComponent::Init(int windowWidth, int windowHeight)
	{
		if (!m_pPhysicsWorld)
		{
			SCION_ERROR("Faile to initialize the physics component");
			return;
		}

		bool bCircle{ m_InitialAttribs.bCircle };

		b2BodyDef bodyDef{};
		bodyDef.type = static_cast<b2BodyType>(m_InitialAttribs.eType);

		bodyDef.position.Set(
			(m_InitialAttribs.position.x + m_InitialAttribs.offset.x) - (windowWidth * 0.5f) +
			((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.x) * 
				m_InitialAttribs.scale.x) * METERS_TO_PIXELS, 
			(m_InitialAttribs.position.y + m_InitialAttribs.offset.y) - (windowHeight * 0.5f) +
			((bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.y) *
				m_InitialAttribs.scale.y) * METERS_TO_PIXELS
		);

		bodyDef.gravityScale = m_InitialAttribs.gravityScale;
		bodyDef.fixedRotation = m_InitialAttribs.bFixdRotation;

		m_pRigidBody = SCION_PHYSICS::MakeSharedBody(m_pPhysicsWorld->CreateBody(&bodyDef));

		if (m_pRigidBody)
		{
			SCION_ERROR("Failed to create rigid body");
			return;
		}

		b2PolygonShape polyShape;
		b2CircleShape circleShape;

		if (bCircle)
		{
			circleShape.m_radius = m_InitialAttribs.radius;
		}
		else if (m_InitialAttribs.bBoxShape)
		{
			polyShape.SetAsBox(
				PIXELS_TO_METERS * m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f,
				PIXELS_TO_METERS * m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f
			);
		}
		else
		{

		}

		b2FixtureDef fixtureDef{};
		if (bCircle)
		{
			fixtureDef.shape = &circleShape;
		}
		else
		{
			fixtureDef.shape = &polyShape;
		}

		fixtureDef.density = m_InitialAttribs.density;
		fixtureDef.friction = m_InitialAttribs.friction;
		fixtureDef.restitution = m_InitialAttribs.restitution;
		fixtureDef.restitutionThreshold = m_InitialAttribs.restitutionThreshold;

		auto pFixture = m_pRigidBody->CreateFixture(&fixtureDef);
		if (!pFixture)
		{
			SCION_ERROR("Fail to create the rigid body");
		}
	}

	void PhysicsComponent::CreatePhysicsLuaBind(sol::state& lua, entt::registry& registry)
	{

	}

}