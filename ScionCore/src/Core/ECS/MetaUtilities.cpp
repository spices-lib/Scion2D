#include "MetaUtilities.h"

namespace SCION_CORE::Utils {

	entt::id_type GetIdType(const sol::table& comp)
	{
		if (!comp.valid())
		{
			SCION_ERROR("Failed to get the type id -- Component has not been exposed to lua!");
			return -1;
		}

		const auto func = comp["type_id"].get<sol::function>();

		return func.valid() ? func().get<entt::id_type>() : -1;
	}
}