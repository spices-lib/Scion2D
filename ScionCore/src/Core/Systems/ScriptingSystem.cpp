#include "ScriptingSystem.h"
#include "Core/ECS/Components/ScriptComponent.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/ECS/Components/SpriteComponent.h"
#include "Core/ECS/Components/AnimationComponent.h"
#include "Core/ECS/Entity.h"
#include "Logger.h"
#include "Core/Scripting/GlmLuaBindings.h"

namespace SCION_CORE::Systems {

	using namespace SCION_CORE::ECS;

	ScriptingSystem::ScriptingSystem(SCION_CORE::ECS::Registry& registry)
		: m_Registry(registry)
		, m_bMainLoaded{ false }
	{
	}

	bool ScriptingSystem::LoadMainScript(sol::state& lua)
	{
		try
		{
			auto result = lua.safe_script_file("./assets/scripts/main.lua");
		}
		catch (sol::error& err)
		{
			SCION_ERROR("Errot loading script {0}", err.what());
			return false;
		}

		sol::table main_lua = lua["main"];

		sol::optional<sol::table> bUpdateExists = main_lua[1];
		if (bUpdateExists == sol::nullopt)
		{
			SCION_ERROR("No update function.");
			return false;
		}

		sol::table update_script = bUpdateExists.value();
		sol::function update = update_script["update"];

		sol::optional<sol::table> bRenderExists = main_lua[2];
		if (bRenderExists == sol::nullopt)
		{
			SCION_ERROR("No render function.");
			return false;
		}

		sol::table render_script = bRenderExists.value();
		sol::function render = render_script["render"];

		SCION_CORE::ECS::Entity mainLuaScript { m_Registry, "main_script", ""};
		mainLuaScript.AddComponent<SCION_CORE::ECS::ScriptComponent>(SCION_CORE::ECS::ScriptComponent{
			.update = update,
			.render = render
		});

		m_bMainLoaded = true;
		return true;
	}

	void ScriptingSystem::Update()
	{
		if (!m_bMainLoaded)
			return;

		auto view = m_Registry.GetRegistry().view<SCION_CORE::ECS::ScriptComponent>();

		for (const auto& e : view)
		{
			SCION_CORE::ECS::Entity entity{ m_Registry, e };
			if (entity.GetName() != "main_script")
				continue;

			auto& script = entity.GetComponent<SCION_CORE::ECS::ScriptComponent>();
			auto error = script.update(entity);

			if (!error.valid())
			{
				sol::error err = error;
				SCION_ERROR("Error running Update script: {0}", err.what());
			}
		}
	}

	void ScriptingSystem::Render()
	{
		if (!m_bMainLoaded)
			return;

		auto view = m_Registry.GetRegistry().view<SCION_CORE::ECS::ScriptComponent>();

		for (const auto& e : view)
		{
			SCION_CORE::ECS::Entity entity{ m_Registry, e };
			if (entity.GetName() != "main_script")
				continue;

			auto& script = entity.GetComponent<SCION_CORE::ECS::ScriptComponent>();
			auto error = script.render(entity);

			if (!error.valid())
			{
				sol::error err = error;
				SCION_ERROR("Error running Render script: {0}", err.what());
			}
		}
	}

	void ScriptingSystem::RegisterLuaBindings(sol::state& lua, SCION_CORE::ECS::Registry& regisry)
	{
		SCION_CORE::Scripting::CreateGLMBindings(lua);

		Registry::CreateLuaRegistryBind(lua, regisry);
		Entity::CreateLuaEntityBind(lua, regisry);
		TransformComponent::CreateLuaTransformBind(lua);
		SpriteComponent::CreateSpriteLuaBind(lua, regisry);
		AnimationComponent::CreateAnimationLuaBind(lua);

		Entity::RegisterMetaComponent<TransformComponent>();
		Entity::RegisterMetaComponent<SpriteComponent>();
		Entity::RegisterMetaComponent<AnimationComponent>();

		Registry::RegisterMetaComponent<TransformComponent>();
		Registry::RegisterMetaComponent<SpriteComponent>();
		Registry::RegisterMetaComponent<AnimationComponent>();
	}

}