#include "ContactListenerBind.h"
#include "Core/ECS/MetaUtilities.h"
#include <Physics/UserData.h>
#include <Logger.h>

using namespace SCION_CORE::Utils;

namespace SCION_CORE::Scripting {

	std::tuple<sol::object, sol::object> ContactListenerBinder::GetUserData(SCION_PHYSICS::ContactListener& contactListener, sol::this_state s)
	{
		auto pUserDataA = contactListener.GetUserDataA();
		auto pUserDataB = contactListener.GetUserDataB();

		if (!pUserDataA || !pUserDataB)
			return std::make_tuple(sol::lua_nil_t{}, sol::lua_nil_t{});

		using namespace entt::literals;

		const auto maybe_any_a = InvokeMetaFunction(
			static_cast<entt::id_type>(pUserDataA->type_id),
			"get_user_data"_hs,
			*pUserDataA, s
		);

		const auto maybe_any_b = InvokeMetaFunction(
			static_cast<entt::id_type>(pUserDataB->type_id),
			"get_user_data"_hs,
			*pUserDataB, s
		);

		if (!maybe_any_a || !maybe_any_b)
			return std::make_tuple(sol::lua_nil_t{}, sol::lua_nil_t{});

		return std::make_tuple(
			maybe_any_a.cast<sol::reference>(),
			maybe_any_b.cast<sol::reference>()
		);
	}

	void ContactListenerBinder::CreateLuaContactListener(sol::state& lua, entt::registry& registry)
	{
		auto& contactListener = registry.ctx().get<std::shared_ptr<SCION_PHYSICS::ContactListener>>();
		if (!contactListener)
		{
			SCION_ERROR("Failed to create contact listener bind");
			return;
		}

		lua.new_usertype<SCION_PHYSICS::ContactListener>(
			"ContactListener",
			sol::no_constructor,
			"get_user_data", [&](sol::this_state s) {
				return GetUserData(*contactListener, s);
			}
		);
	}

}