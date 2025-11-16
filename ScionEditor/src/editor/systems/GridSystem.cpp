#include "GridSystem.h"
#include <Core/ECS/MainRegistry.h>
#include <Rendering/Core/RectBatchRenderer.h>
#include <Rendering/Essentials/Primitives.h>
#include <Rendering/Essentials/Vertex.h>
#include <Rendering/Core/Camera.h>
#include <Core/Resources/AssetManager.h>

namespace SCION_EDITOR {

	GridSystem::GridSystem()
		: m_pBatchRenderer{ std::make_unique<SCION_RENDERING::RectBatchRenderer>() }
	{}

	void GridSystem::Update(SCION_RENDERING::Camera2D& camera)
	{
		auto& assetManager = SCION_CORE::ECS::MainRegistry::GetInstance();

		auto& pColorShader = assetManager.GetAssetManager().GetShader("color");
		pColorShader.SetUniformMat4("uProjection", camera.GetCameraMatrix());

		m_pBatchRenderer->Begin();


		int tileWidth{ 16 }, tileHeight{ 16 }, canvasWidth{ 640 }, canvasHeight{ 480 };

		int cols = canvasWidth / tileWidth;
		int rows = canvasHeight / tileHeight;

		SCION_RENDERING::Color color{};

		for (int row = 0; row < rows; row++)
		{
			for (int col = 0; col < cols; col++)
			{
				if ((col - row) % 2 == 0 && (row - col) % 2 == 0)
					color = { 125, 125, 125, 70 };
				else
					color = { 200, 200, 200, 70 };

				SCION_RENDERING::Rect rect {
					.position = glm::vec2{ static_cast<float>(col * tileWidth), static_cast<float>(row * tileHeight) },
					.width = static_cast<float>(tileWidth),
					.height = static_cast<float>(tileHeight),
					.color = color
				};

				m_pBatchRenderer->AddRect(rect);
			}
		}

		m_pBatchRenderer->End();
		m_pBatchRenderer->Render();

		pColorShader.Disable();
	}
}