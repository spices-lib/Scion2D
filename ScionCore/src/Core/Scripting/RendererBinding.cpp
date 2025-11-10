#include "RendererBinding.h"
#include <Rendering/Essentials/Primitives.h>
#include <Rendering/Core/Camera.h>
#include <Rendering/Core/Renderer.h>
#include "Core/ECS/Registry.h"
#include <Logger.h>
#include "Core/Resources/AssetManager.h"

using namespace SCION_RENDERING;
using namespace SCION_RESOURCE;

namespace SCION_CORE::Scripting {

	void RendererBinder::CreateRenderingBinding(sol::state& lua, SCION_CORE::ECS::Registry& registry)
	{
		auto& camera = registry.GetContext<std::shared_ptr<Camera2D>>();
		auto& assetMananger = registry.GetContext<std::shared_ptr<AssetManager>>();

		lua.new_usertype<Line>(
			"Line",
			sol::call_constructor,
			sol::factories(
				[](const glm::vec2& p1, const glm::vec2& p2, const Color& color) {
					return Line{ .p1 = p1, .p2 = p2, .color = color };
				}
			),
			"p1", &Line::p1,
			"p2", &Line::p2,
			"color", &Line::color
		);

		lua.new_usertype<Rect>(
			"Rect",
			sol::call_constructor,
			sol::factories(
				[](const glm::vec2& position, float width, float height, const Color& color) {
					return Rect{ .position = position, .width = width,.height = height, .color = color };
				}
			),
			"position", &Rect::position,
			"width", &Rect::width,
			"height", &Rect::height,
			"color", &Rect::color
		);

		lua.new_usertype<Text>(
			"Text",
			sol::call_constructor,
			sol::factories(
				[&](const glm::vec2& position, const std::string& textStr, const std::string& fontName, float wrap, const Color& color) {
					auto pFont = assetMananger->GetFont(fontName);
					if (!pFont)
					{
						SCION_ERROR("Failed to get font {}", fontName);
						return Text{};
					}

					return Text{
						.position = position,
						.textStr = textStr,
						.wrap = wrap,
						.pFont = pFont,
						.color = color
					};
				}
			),
			"position", &Text::position,
			"textStr", &Text::textStr,
			"wrap", &Text::wrap,
			"color", &Text::color
		);

		auto& renderer = registry.GetContext<std::shared_ptr<Renderer>>();
		if (!renderer)
		{
			SCION_ERROR("Failed to bind renderer to lua");
			return;
		}

		lua.set_function(
			"DrawRect", sol::overload(
				[&](const Rect& rect) {
					renderer->DrawRect(rect);
				},
				[&](const glm::vec2& position, float width, float height, const Color& color) {
					renderer->DrawRect(position, width, height, color);
				}
			)
		);

		lua.set_function(
			"DrawLine", sol::overload(
				[&](const Line& line) {
					renderer->DrawLine(line);
				},
				[&](const glm::vec2& p1, const glm::vec2& p2, const Color& color) {
					renderer->DrawLine(p1, p2, color);
				}
			)
		);

		lua.set_function(
			"DrawText", [&](const Text& text) {
				renderer->DrawText2D(text);
			}
		);

		lua.new_usertype<Camera2D>(
			"Camera", 
			sol::no_constructor,
			"get", [&]() { return *camera; },
			"position", [&]() { return camera->GetPosition(); },
			"set_position", [&](const glm::vec2& newPosition) {
				camera->SetPosition(newPosition);
			},
			"scale", [&]() {
				camera->GetScale();
			},
			"set_scale", [&](float scale) {
				camera->SetScale(scale);
			}
		);
	}

}