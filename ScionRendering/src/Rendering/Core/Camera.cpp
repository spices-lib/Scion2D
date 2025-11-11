#include "Camera.h"

namespace SCION_RENDERING {

	Camera2D::Camera2D()
		: Camera2D(640, 480)
	{}

	Camera2D::Camera2D(int width, int height)
		: m_Width(width), m_Height(height), m_Scale(1.0f)
		, m_Position(glm::vec2(0.0f))
		, m_ScreenOffset{ 0.0f }
		, m_CameraMatrix(1.0f)
		, m_OrthoProjection(1.0f)
		, m_bNeedsUpdate(true)
	{
		m_OrthoProjection = glm::ortho(0.0f, static_cast<float>(m_Width), 0.0f, static_cast<float>(m_Height), -1.0f, 1.0f);

		Update();
	}

	void Camera2D::Update()
	{
		if (!m_bNeedsUpdate)
			return;

		glm::vec3 translate(-m_Position.x, -m_Position.y, 0.0f);
		m_CameraMatrix = glm::translate(m_OrthoProjection, translate);

		glm::vec3 scale(m_Scale, m_Scale, 0.0f);
		m_CameraMatrix *= glm::scale(glm::mat4(1.0f), scale);

		m_bNeedsUpdate = false;
	}

	glm::vec2 Camera2D::ScreenCoordsToWorld(const glm::vec2 screenCoords)
	{
		glm::vec2 worldCoords{ screenCoords };

		worldCoords -= m_ScreenOffset;
		worldCoords /= m_Scale;
		worldCoords += m_Position;

		return worldCoords;
	}

	glm::vec2 Camera2D::WorldCoordsToScreen(const glm::vec2 worldCoords)
	{
		glm::vec2 screenCoords{ worldCoords };

		screenCoords += m_ScreenOffset;

		screenCoords *= m_Scale;

		screenCoords -= m_ScreenOffset;

		return screenCoords;
	}
}