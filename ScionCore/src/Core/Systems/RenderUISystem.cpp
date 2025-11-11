#include "RenderUISystem.h"
#include "Core/ECS/Components/TextComponent.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/Resources/AssetManager.h"
#include <Logger.h>
#include <Rendering/Essentials/Font.h>

using namespace SCION_CORE::ECS;
using namespace SCION_RESOURCE;

namespace SCION_CORE::Systems {

	RenderUISystem::RenderUISystem(SCION_CORE::ECS::Registry& registry)
		: m_Registry(registry)
		, m_pSpriteRenderer{ std::make_unique<SCION_RENDERING::SpriteBatchRenderer>()}
		, m_pTextRenderer{ std::make_unique<SCION_RENDERING::TextBatchRenderer>() }
		, m_pCamera2D{ nullptr }
	{
		m_pCamera2D = std::make_unique<SCION_RENDERING::Camera2D>(640, 480);
		m_pCamera2D->Update();
	}

	void RenderUISystem::Update(entt::registry& registry)
	{
		auto& assetManager = m_Registry.GetContext<std::shared_ptr<AssetManager>>();
		auto pFontShader = assetManager->GetShader("font");

		auto cam_mat = m_pCamera2D->GetCameraMatrix();
		auto textView = registry.view<TextComponent, TransformComponent>();

		pFontShader.Enable();
		pFontShader.SetUniformMat4("uProjection", cam_mat);

		m_pTextRenderer->Begin();

		for (auto& entity : textView)
		{
			const auto& text = textView.get<TextComponent>(entity);

			if (text.sFontName.empty() || text.bHidden)
				continue;

			const auto& pFont = assetManager->GetFont(text.sFontName);

			const auto& transform = textView.get<TransformComponent>(entity);

			glm::mat4 model{ 1.0f };

			if (transform.rotation != 0.0f || transform.scale.x != 1.0f || transform.scale.y != 1.0f)
			{
				model = glm::translate(model, glm::vec3{transform.position, 0.0f});

				model = glm::translate(model, glm::vec3{pFont->GetFontSize() * transform.scale.x * 0.5f, pFont->GetFontSize()* transform.scale.y * 0.5f, 0.0f});

				model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3{0.0f, 0.0f, 1.0f});

				model = glm::translate(model, glm::vec3{pFont->GetFontSize()* transform.scale.x * -0.5f, pFont->GetFontSize()* transform.scale.y * -0.5f, 0.0f});

				model = glm::scale(model, glm::vec3{transform.scale, 1.0f});

				model = glm::translate(model, glm::vec3{-transform.position, 0.0f});
			}

			m_pTextRenderer->AddText(text.sTextStr, pFont, transform.position, text.padding, text.wrap, text.color, model);
		}

		m_pTextRenderer->End();
		m_pTextRenderer->Render();

		pFontShader.Disable();
	}

}