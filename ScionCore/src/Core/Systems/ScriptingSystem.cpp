#include "ScriptingSystem.h"
#include "Core/ECS/Components/ScriptComponent.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/ECS/Components/SpriteComponent.h"
#include "Core/ECS/Components/AnimationComponent.h"
#include "Core/ECS/Components/CircleColliderComponent.h"
#include "Core/ECS/Components/BoxColliderComponent.h"
#include "Core/ECS/Components/PhysicalComponent.h"
#include "Core/ECS/Components/TextComponent.h"
#include "Core/ECS/Entity.h"
#include "Logger.h"
#include "Core/Scripting/GlmLuaBindings.h"
#include "Core/Resources/InputManager.h"
#include "Core/Resources/AssetManager.h"
#include "Core/Scripting/SoundBinding.h"
#include "Core/Scripting/RendererBinding.h"
#include "Core/CoreUtilities/FollowCamera.h"
#include "Timer.h"
#include "RandomGenerator.h"
#include "Core/Scripting/UserDataBinding.h"

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

		auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();
		lua->collect_garbage();
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

	auto create_timer = [](sol::state& lua) {
		using namespace SCION_UTL;
		lua.new_usertype<Timer>(
			"Timer",
			sol::call_constructor,
			sol::constructors<Timer()>(),
			"start", &Timer::Start,
			"stop", &Timer::Stop,
			"pause", &Timer::Pause,
			"resume", &Timer::Resume,
			"is_paused", &Timer::IsPause,
			"is_running", &Timer::IsRunning,
			"elapsed_ms", &Timer::ElapsedMS,
			"elasped_sec", &Timer::ELapsedSec,
			"restart", [](Timer& timer) {
				if (timer.IsRunning())
				{
					timer.Stop();
				}

				timer.Start();
			}
		);
	};

	void ScriptingSystem::RegisterLuaBindings(sol::state& lua, SCION_CORE::ECS::Registry& regisry)
	{
		SCION_CORE::Scripting::CreateGLMBindings(lua);
		SCION_CORE::InputManager::CreateLuaInputBindings(lua, regisry);
		SCION_RESOURCE::AssetManager::CreateLuaAssetManager(lua, regisry);
		SCION_CORE::Scripting::SoundBinder::CreateSoundBind(lua, regisry);
		SCION_CORE::Scripting::RendererBinder::CreateRenderingBinding(lua, regisry);
		SCION_CORE::Scripting::UserDataBinder::CreateLuaUserData(lua);
		SCION_CORE::FollowCamera::CreateLuaFollowCamera(lua, regisry);

		create_timer(lua);

		Registry::CreateLuaRegistryBind(lua, regisry);
		Entity::CreateLuaEntityBind(lua, regisry);
		TransformComponent::CreateLuaTransformBind(lua);
		SpriteComponent::CreateSpriteLuaBind(lua, regisry);
		AnimationComponent::CreateAnimationLuaBind(lua);
		CircleColliderComponent::CreateLuaCircleColliderBind(lua);
		BoxColliderComponent::CreateLuaBoxColloderBind(lua);
		PhysicsComponent::CreatePhysicsLuaBind(lua, regisry.GetRegistry());
		TextComponent::CreateLuaTextBindings(lua);

		Entity::RegisterMetaComponent<TransformComponent>();
		Entity::RegisterMetaComponent<SpriteComponent>();
		Entity::RegisterMetaComponent<AnimationComponent>();
		Entity::RegisterMetaComponent<CircleColliderComponent>();
		Entity::RegisterMetaComponent<BoxColliderComponent>();
		Entity::RegisterMetaComponent<PhysicsComponent>();
		Entity::RegisterMetaComponent<TextComponent>();

		Registry::RegisterMetaComponent<TransformComponent>();
		Registry::RegisterMetaComponent<SpriteComponent>();
		Registry::RegisterMetaComponent<AnimationComponent>();
		Registry::RegisterMetaComponent<CircleColliderComponent>();
		Registry::RegisterMetaComponent<BoxColliderComponent>();
		Registry::RegisterMetaComponent<PhysicsComponent>();
		Registry::RegisterMetaComponent<TextComponent>();

		SCION_CORE::Scripting::UserDataBinder::register_meta_user_data<ObjectData>();
	}

	void ScriptingSystem::RegisterLuaFunctions(sol::state& lua)
	{
		lua.set_function(
			"run_script", [&](const std::string& path) {
				try
				{
					lua.safe_script_file(path);
				}
				catch (const sol::error& error)
				{
					SCION_ERROR("Error loading Lua Script: {0}", error.what());
					return false;
				}

				return true;
			}
		);

		lua.new_usertype<SCION_UTL::RandomGenerator>(
			"Random",
			sol::call_constructor,
			sol::constructors<SCION_UTL::RandomGenerator(uint32_t, uint32_t), SCION_UTL::RandomGenerator()>(),
			"get_float", &SCION_UTL::RandomGenerator::GetFloat,
			"get_int", &SCION_UTL::RandomGenerator::GetInt
		);
	}

}