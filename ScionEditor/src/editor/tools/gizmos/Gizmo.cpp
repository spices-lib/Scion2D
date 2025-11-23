#include "Gizmo.h"
#include <Core/ECS/MainRegistry.h>
#include <Core/Resources/AssetManager.h>
#include <Core/ECS/Entity.h>
#include <Core/CoreUtilities/CoreUtilities.h>
#include "editor/utilities/EditorUtilities.h"
#include "Logger.h"
#include <Rendering/Essentials/Vertex.h>

using namespace SCION_CORE::ECS;

namespace SCION_EDITOR {

	Gizmo::Gizmo()
		: Gizmo()
	{
	}

	Gizmo::Gizmo(const GizmoAxisParams& xAxisParams, const GizmoAxisParams& yAxisParams, bool bOneAxis)
		: AbstractTool()
		, m_pXAxisParams{ nullptr }
		, m_pYAxisParams{ nullptr }
		, m_pBatchRenderer{ std::make_unique<SCION_RENDERING::SpriteBatchRenderer>() }
		, m_SelectedEntity{ entt::null }
		, m_LastMousePos{ 0.0f }
		, m_bOverXAxis{ false }
		, m_bOverYAxis{ false }
		, m_bHoldingX{ false }
		, m_bHoldingY{ false }
		, m_bHidden{ false }
		, m_bOnlyOneAxis{ bOneAxis }
	{
		m_pXAxisParams = std::make_unique<GizmoAxisParams>(xAxisParams);

		if (m_bOnlyOneAxis)
			return;

		m_pYAxisParams = std::make_unique<GizmoAxisParams>(yAxisParams);
	}

	void Gizmo::Update(Canvas& canvas)
	{
		m_LastMousePos = GetMouseScreenCoords();

		AbstractTool::Update(canvas);
	}

	void Gizmo::SetSelectedEntity(entt::entity entity)
	{
		m_SelectedEntity = entity;
		if (m_SelectedEntity != entt::null && m_pRegistry)
		{
			Entity ent{ *m_pRegistry, entity };
			SetGizmoPosition(ent);
		}
	}

	void Gizmo::Hide()
	{
		if (m_pXAxisParams)
		{
			m_pXAxisParams->sprite.bHidden = true;
		}

		if (m_pYAxisParams)
		{
			m_pYAxisParams->sprite.bHidden = true;
		}

		m_bHidden = true;
	}

	void Gizmo::Show()
	{
		if (m_bHidden && m_SelectedEntity != entt::null)
		{
			if (m_pXAxisParams)
			{
				m_pXAxisParams->sprite.bHidden = false;
			}

			if (m_pYAxisParams)
			{
				m_pYAxisParams->sprite.bHidden = false;
			}

			m_bHidden = false;
		}
	}

	void Gizmo::Init(const std::string& sXAxisTexture, const std::string& sYAxisTexture)
	{
		m_pXAxisParams->sprite.texture_name = sXAxisTexture;
		auto pXAxisTexture = MainRegistry::GetInstance().GetAssetManager().GetTexture(sXAxisTexture);

		m_pXAxisParams->sprite.width = pXAxisTexture.GetWidth();
		m_pXAxisParams->sprite.height = pXAxisTexture.GetHeight();
		m_pXAxisParams->sprite.color = m_pXAxisParams->axisColor;

		SCION_CORE::GenerateUVs(m_pXAxisParams->sprite, pXAxisTexture.GetWidth(), pXAxisTexture.GetHeight());

		if (!m_bOnlyOneAxis)
		{
			m_pYAxisParams->sprite.texture_name = sXAxisTexture;
			auto pYAxisTexture = MainRegistry::GetInstance().GetAssetManager().GetTexture(sYAxisTexture);

			m_pYAxisParams->sprite.width = pYAxisTexture.GetWidth();
			m_pYAxisParams->sprite.height = pYAxisTexture.GetHeight();
			m_pYAxisParams->sprite.color = m_pYAxisParams->axisColor;

			SCION_CORE::GenerateUVs(m_pYAxisParams->sprite, pYAxisTexture.GetWidth(), pYAxisTexture.GetHeight());

		}

		Hide();
	}

	void Gizmo::ExamineMousePosition()
	{
		const auto& mousePos = GetMouseWorldCoords();

		const auto& xAxisTransform = m_pXAxisParams->transform;
		auto& xAxisSprit = m_pXAxisParams->sprite;

		if (mousePos.x >= xAxisTransform.position.x)
		{
			xAxisSprit.color = m_pXAxisParams->axisHoveredColor;
			m_bOverXAxis = true;
		}
		else if (m_bHoldingX)
		{
			xAxisSprit.color = m_pXAxisParams->axisHoveredColor;
			m_bOverXAxis = true;
		}
		else if (m_bHoldingY)
		{
			xAxisSprit.color = m_pXAxisParams->axisDisabledColor;
			m_bOverXAxis = false;
		}
		else
		{
			xAxisSprit.color = m_pXAxisParams->axisColor;
			m_bOverXAxis = false;
		}

		if (m_bOnlyOneAxis)
			return;
	}

	float Gizmo::GetDetalX()
	{
		if (!IsOverTilemapWindow() || OutOfBounds())
			return 0.0f;

		if (!m_bOverXAxis && !m_bHoldingX)
			return 0.0f;

		if (MouseBtnPressed(AbstractTool::EMouseButton::LEFT) && MouseMoving())
		{
			m_bHoldingX = true;
			return std::ceil((GetMouseScreenCoords().x - m_LastMousePos.x) / m_pCamera->GetScale());
		}

		if (MouseBtnJustReleased(AbstractTool::EMouseButton::LEFT))
		{
			m_bHoldingX = false;
		}

		return 0.0f;
	}

	float Gizmo::GetDetalY()
	{
		if (!IsOverTilemapWindow() || OutOfBounds() || m_bOnlyOneAxis)
			return 0.0f;

		if (!m_bOverYAxis && !m_bHoldingY)
			return 0.0f;

		if (MouseBtnPressed(AbstractTool::EMouseButton::LEFT) && MouseMoving())
		{
			m_bHoldingY = true;
			return std::ceil((GetMouseScreenCoords().x - m_LastMousePos.x) / m_pCamera->GetScale());
		}

		if (MouseBtnJustReleased(AbstractTool::EMouseButton::LEFT))
		{
			m_bHoldingY = false;
		}

		return 0.0f;
	}

	void Gizmo::SetGizmoPosition(SCION_CORE::ECS::Entity& selectedEntity)
	{
		float spriteWidth{ 0.0f };
		float spriteHeight{ 0.0f };

		if (const auto* sprite = selectedEntity.TryGetComponent<SpriteComponent>())
		{
			spriteWidth = sprite->width;
			spriteHeight = sprite->height;
		}

		const auto& selectedTransform = selectedEntity.GetComponent<TransformComponent>();

		if (!m_bOnlyOneAxis)
		{
			m_pXAxisParams->transform.position = selectedTransform.position + m_pXAxisParams->axisOffset + glm::vec2{
				spriteWidth * selectedTransform.scale.x * 0.5f,
				spriteHeight * selectedTransform.scale.y * 0.5f - m_pXAxisParams->sprite.height * m_pXAxisParams->transform.scale.y * 0.5f
			};

			m_pYAxisParams->transform.position = selectedTransform.position + m_pXAxisParams->axisOffset + glm::vec2{
				spriteWidth* selectedTransform.scale.x * 0.5f,
				spriteHeight* selectedTransform.scale.y * 0.5f - m_pXAxisParams->sprite.height * m_pXAxisParams->transform.scale.y * 0.5f
			};
		}
		else
		{
			m_pXAxisParams->transform.position = selectedTransform.position + m_pXAxisParams->axisOffset + glm::vec2{
				spriteWidth * selectedTransform.scale.x * 0.5f,
				spriteHeight * selectedTransform.scale.y * 0.5f - m_pXAxisParams->sprite.height * m_pXAxisParams->transform.scale.y * 0.5f
			};

			m_pYAxisParams->transform.position = selectedTransform.position + m_pXAxisParams->axisOffset + glm::vec2{
				spriteWidth* selectedTransform.scale.x * 0.5f,
				spriteHeight* selectedTransform.scale.y * 0.5f - m_pXAxisParams->sprite.height * m_pXAxisParams->transform.scale.y * 0.5f
			};
		}
	}

}