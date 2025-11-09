#include "RenderSystem.h"
#include "Core/Resources/AssetManager.h"
#include "Core/ECS/Components/SpriteComponent.h"
#include "Core/ECS/Components/TransformComponent.h"
#include <Rendering/Core/Camera.h>
#include <Rendering/Essentials/Shader.h>
#include <Logger.h>

namespace SCION_CORE::Systems {

	using namespace SCION_RENDERING;
	using namespace SCION_CORE::ECS;
	using namespace SCION_RESOURCE;

	RenderSystem::RenderSystem(SCION_CORE::ECS::Registry& registry)
		: m_Registry(registry)
		, m_pBatchRenderer{ nullptr }
	{
		m_pBatchRenderer = std::make_unique<SpriteBatchRenderer>();
	}

	void RenderSystem::Update()
	{
		auto& assetManager = m_Registry.GetContext<std::shared_ptr<AssetManager>>();

		auto& spriteShader = assetManager->GetShader("basic");

		if (spriteShader.ShaderProgramID() == 0)
		{
			SCION_ERROR("Sprite shader program error");
			return;
		}

		spriteShader.Enable();

		auto& camera = m_Registry.GetContext<std::shared_ptr<Camera2D>>();
		spriteShader.SetUniformMat4("uProjection", camera->GetCameraMatrix());

		m_pBatchRenderer->Begin();

		auto view = m_Registry.GetRegistry().view<SpriteComponent, TransformComponent>();

		for (const auto& e : view)
		{
			const auto& sprite = view.get<SpriteComponent>(e);
			const auto& transform = view.get<TransformComponent>(e);

			if (sprite.texture_name.empty() || sprite.bHidden)
			{
				continue;
			}

			const auto& texture = assetManager->GetTexture(sprite.texture_name);
			if (texture.GetID() == 0)
			{
				SCION_ERROR("Texture {0} was not created correctly!", sprite.texture_name);
				return;
			}

			glm::vec4 spriteRect{ transform.position.x, transform.position.y, sprite.width, sprite.height };
			glm::vec4 uvRect{ sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height };

			glm::mat4 model{ 1.0f };

			if (transform.rotation != 0.0f || transform.scale.x != 1.0f || transform.scale.y != 1.0f)
			{
				model = glm::translate(model, glm::vec3{transform.position, 0.0f});

				model = glm::translate(model, glm::vec3{sprite.width * transform.scale.x * 0.5f, sprite.height * transform.scale.y * 0.5f, 0.0f});

				model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3{0.0f, 0.0f, 1.0f});

				model = glm::translate(model, glm::vec3{sprite.width * transform.scale.x * -0.5f, sprite.height * transform.scale.y * -0.5f, 0.0f});

				model = glm::scale(model, glm::vec3{transform.scale, 1.0f});

				model = glm::translate(model, glm::vec3{-transform.position, 0.0f});
			}

			m_pBatchRenderer->AddSprite(spriteRect, uvRect, texture.GetID(), sprite.layer, model, sprite.color);
		}

		m_pBatchRenderer->End();

		m_pBatchRenderer->Render();

		spriteShader.Disable();
	}

}