#include "ScaleGizmo.h"
#include "editor/utilities/EditorUtilities.h"
#include "Rendering/Core/Batcher.h"
#include "Rendering/Core/Camera.h"
#include "Core/ECS/MainRegistry.h"
#include "Core/CoreUtilities/CoreUtilities.h"
#include "Core/ECS/Entity.h"
#include <Logger.h>
#include <Core/Resources/AssetManager.h>

using namespace SCION_CORE::ECS;

namespace SCION_EDITOR {

	ScaleGizmo::ScaleGizmo()
		: Gizmo()
	{
		Init("S2D_x_axis_translate", "S2D_y_axis_translate");
	}

	void ScaleGizmo::Update(Canvas& canvas)
	{
		Gizmo::Update(canvas);

		if (m_SelectedEntity == entt::null || !m_pRegistry)
		{
			Hide();
			return;
		}

		Show();

		Entity selectedEntity{ *m_pRegistry, m_SelectedEntity };
		auto& selectedTransform = selectedEntity.GetComponent<TransformComponent>();

		selectedTransform.scale.x += GetDetalX();
		selectedTransform.scale.y += GetDetalX();

		SetGizmoPosition(selectedEntity);

		ExamineMousePosition();
	}

	void ScaleGizmo::Draw(Canvas& canvas)
	{
		auto& pShader = SCION_CORE::ECS::MainRegistry::GetInstance().GetAssetManager().GetShader("basic");

		pShader.Enable();
		auto camMat = m_pCamera->GetCameraMatrix();
		pShader.SetUniformMat4("uProjection", camMat);

		m_pBatchRenderer->Begin();
		const auto& xAxisSprite = m_pXAxisParams->sprite;
		const auto& xAxisTransform = m_pXAxisParams->transform;

		if (!xAxisSprite.bHidden)
		{
			glm::vec4 xAxisPosition{
				xAxisTransform.position.x,
					xAxisTransform.position.y,
					xAxisSprite.width* xAxisTransform.scale.x,
					xAxisSprite.height* xAxisTransform.scale.y
			};

			glm::vec4 xAxisUVs
			{
				xAxisSprite.uvs.u,
					xAxisSprite.uvs.v,
					xAxisSprite.uvs.uv_width,
					xAxisSprite.uvs.uv_height
			};

			const auto pXAxisTexture = MainRegistry::GetInstance().GetAssetManager().GetTexture(xAxisSprite.texture_name);

			m_pBatchRenderer->AddSprite(xAxisPosition, xAxisUVs, 99);
		}

		pShader.Disable();
	}

}