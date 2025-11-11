#include "FollowCamera.h"
#include "Core/ECS/Components/TransformComponent.h"

using namespace SCION_CORE::ECS;

namespace SCION_CORE {

	FollowCamera::FollowCamera(SCION_RENDERING::Camera2D& camera, const ECS::Entity& entity, const FollowCamParams& params)
		: m_Camera{ camera }
		, m_Params{ params }
		, m_Entity{ entity }
	{
		m_Params.minX *= m_Params.scale;
		m_Params.minY *= m_Params.scale;
		m_Params.maxX *= m_Params.scale;
		m_Params.maxY *= m_Params.scale;

		m_Params.springback = std::clamp(m_Params.springback, 0.0f, 1.0f);
	}

	void FollowCamera::Update()
	{
		const auto& transform = m_Entity.GetComponent<TransformComponent>();
		const auto& camWidth = m_Camera.GetWidth();
		const auto& camHeight = m_Camera.GetHeight();
		const auto& camPos = m_Camera.GetPosition();

		glm::vec2 newCamPosition { 0.0f };

		newCamPosition.x = (transform.position.x - camWidth / (2 * m_Params.scale)) * m_Params.scale;
		newCamPosition.y = (transform.position.y - camHeight / (2 * m_Params.scale)) * m_Params.scale;

		newCamPosition.x = std::clamp(newCamPosition.x, m_Params.minX, m_Params.maxX - camWidth);
		newCamPosition.y = std::clamp(newCamPosition.y, m_Params.minY, m_Params.maxY - camHeight);

		m_Camera.SetPosition(
			glm::vec2{
				std::lerp(camPos.x, newCamPosition.x, m_Params.springback),
				std::lerp(camPos.y, newCamPosition.y, m_Params.springback)
			}
		);
	}

	void FollowCamera::CreateLuaFollowCamera(sol::state& lua, ECS::Registry& registry)
	{

	}

}