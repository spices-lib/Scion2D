#include "TileTool.h"
#include <Logger.h>
#include "editor/utilities/EditorUtilities.h"
#include <Core/ECS/MainRegistry.h>
#include <Core/Resources/AssetManager.h>
#include <Rendering/Core/SpriteBatchRenderer.h>
#include <Rendering/Core/Camera.h>
#include <Core/ECS/Components/TileComponent.h>

constexpr int MOUSE_SPRITE_LAYER = 10;

using namespace SCION_CORE::ECS;

namespace SCION_EDITOR {

	uint32_t TileTool::CheckForTile(const glm::vec2& position)
	{
		if (!m_pRegistry)
			return entt::null;

		auto tileView = m_pRegistry->GetRegistry().view<TileComponent, TransformComponent>();

		for (auto entity : tileView)
		{
			Entity tile{ *m_pRegistry, entity };
			const auto& transform = tile.GetComponent<TransformComponent>();
			const auto& sprite = tile.GetComponent<SpriteComponent>();

			if (position.x >= transform.position.x && position.x < transform.position.x + sprite.width * transform.scale.x &&
			    position.y >= transform.position.y && position.y < transform.position.y + sprite.height * transform.scale.y && 
				m_pMouseTile->sprite.layer == sprite.layer
			)
			{
				return static_cast<uint32_t>(entity);
			}
		}

		return entt::null;
	}

	SCION_CORE::ECS::Entity TileTool::CreateEntity()
	{
		return Entity{ *m_pRegistry, "", "" };
	}

	SCION_CORE::ECS::Entity TileTool::CreateEntity(uint32_t id)
	{
		return Entity{ *m_pRegistry, static_cast<entt::entity>(id) };
	}

	void TileTool::DrawMouseSprite()
	{
		m_pBatchRenderer->Begin();
		const auto& sprite = m_pMouseTile->sprite;
		const auto& transform = m_pMouseTile->transform;

		glm::vec4 position{
			transform.position.x,
			transform.position.y,
			sprite.width * transform.scale.x,
			sprite.height * transform.scale.y
		};

		glm::vec4 uvs{ sprite.uvs.u, sprite.uvs.v, sprite.uvs.uv_width, sprite.uvs.uv_height };

		const auto pTexture = MainRegistry::GetInstance().GetAssetManager().GetTexture(sprite.texture_name);

		m_pBatchRenderer->AddSprite(position, uvs, pTexture.GetID(), MOUSE_SPRITE_LAYER, glm::mat4{1.0f}, sprite.color);

		m_pBatchRenderer->End();
		m_pBatchRenderer->Render();
	}

	void TileTool::ExamineMousePosition()
	{
		if (!m_pCamera)
			return;

		const auto& mouseWorldPos = GetMouseWorldCoords();
		auto& transform = m_pMouseTile->transform;

		const float cameraScale = m_pCamera->GetScale();
		bool bIsOffset{ true };

		if (m_bGridSnap)
		{
			glm::vec2 mouseGrid{0.0f};
			if (mouseWorldPos.x >= 0.0f)
			{
				mouseGrid.x = static_cast<int>(mouseWorldPos.x / (m_MouseRect.x * transform.scale.x) * cameraScale);
			}
			if (mouseWorldPos.y >= 0.0f)
			{
				mouseGrid.y = static_cast<int>(mouseWorldPos.y / (m_MouseRect.y * transform.scale.y) * cameraScale);
			}
			transform.position.x = std::floor((mouseGrid.x / cameraScale)) * m_MouseRect.x * transform.position.x;
			transform.position.x = std::floor((mouseGrid.y / cameraScale)) * m_MouseRect.y * transform.position.y;

			m_GridCoords.x = mouseGrid.x / cameraScale;
			m_GridCoords.y = mouseGrid.y / cameraScale;

			SetMouseWorldCoords(transform.position);
		}
		else
		{
			transform.position = bIsOffset ? mouseWorldPos - m_MouseRect * 0.5f : mouseWorldPos;
		}
	}

	TileTool::TileTool()
		: AbstractTool()
		, m_pBatchRenderer{ std::make_shared<SCION_RENDERING::SpriteBatchRenderer>() }
		, m_pMouseTile{ std::make_shared<Tile>() }
		, m_MouseRect{ 16.f }
		, m_GridCoords{ 0.0f }
		, m_bGridSnap{ true }
	{

	}

	void TileTool::Update(Canvas& canvas)
	{
		AbstractTool::Update(canvas);
		ExamineMousePosition();
	}

	void TileTool::ClearMouseTextureData()
	{
		m_pMouseTile->sprite = SpriteComponent{};
	}

	void TileTool::LoadSpriteTextureData(const std::string& textureName)
	{
		m_pMouseTile->sprite = SpriteComponent{
			.width = m_MouseRect.x,
			.height = m_MouseRect.y,
			.color = SCION_RENDERING::Color{ 255, 255, 255, 255 },
			.start_x = 0,
			.start_y = 0,
			.layer = 0,
			.texture_name = textureName
		};

		auto pTexture = MainRegistry::GetInstance().GetAssetManager().GetTexture(textureName);
		m_pMouseTile->sprite.generate_uvs(pTexture.GetWidth(), pTexture.GetHeight());
	}

	const std::string& TileTool::GetSpriteTexture() const
	{
		if (!m_pMouseTile)
			return std::string{""};

		return m_pMouseTile->sprite.texture_name;
	}

	void TileTool::SetBoxColloder(int width, int height, const glm::vec2& offset)
	{
		m_pMouseTile->boxCollider.width = width;
		m_pMouseTile->boxCollider.height = height;
		m_pMouseTile->boxCollider.offset = offset;
	}

	void TileTool::SetSpriteLayer(int layer)
	{
		m_pMouseTile->sprite.layer = layer;
	}

	void TileTool::SetScale(const glm::vec2& scale)
	{
		m_pMouseTile->transform.scale = scale;
	}

	void TileTool::SetAnimation(int numFrames, int frameRate, bool bVertical, bool bLooped, int frameOffset)
	{
		m_pMouseTile->animation.numFrames = numFrames;
		m_pMouseTile->animation.frameRate = frameRate;
		m_pMouseTile->animation.bVertical = bVertical;
		m_pMouseTile->animation.bLooped = bLooped;
		m_pMouseTile->animation.frameOffset = frameOffset;
	}

	void TileTool::SetSpriteUVs(int staerX, int startY)
	{
		m_pMouseTile->sprite.start_x = staerX;
		m_pMouseTile->sprite.start_y = startY;
		m_pMouseTile->sprite.uvs.u = staerX * m_pMouseTile->sprite.uvs.uv_width;
		m_pMouseTile->sprite.uvs.v = startY * m_pMouseTile->sprite.uvs.uv_height;
	}

	void TileTool::SetSpriteRect(const glm::vec2& spriteRect)
	{
		if (m_pMouseTile->sprite.texture_name.empty())
			return;

		m_MouseRect = spriteRect;
		auto& sprite = m_pMouseTile->sprite;
		sprite.width = m_MouseRect.x;
		sprite.height = m_MouseRect.y;

		auto pTexture = MainRegistry::GetInstance().GetAssetManager().GetTexture(sprite.texture_name);
		sprite.generate_uvs(pTexture.GetWidth(), pTexture.GetHeight());
	}

	void TileTool::SetCollider(bool bCollider)
	{
		m_pMouseTile->bColloder = bCollider;
	}

	void TileTool::SetCircle(bool bCircle)
	{
		m_pMouseTile->bCircle = bCircle;
	}

	void TileTool::SetAnimation(bool bAnimation)
	{
		m_pMouseTile->bAnimation = bAnimation;
	}

	void TileTool::SetPhysics(bool bPhysics)
	{
		m_pMouseTile->bPhysics = bPhysics;
	}

	const bool TileTool::SpriteValid() const
	{
		return !m_pMouseTile->sprite.texture_name.empty();
	}

}