#include "GlmLuaBindings.h"
#include <glm/glm.hpp>

namespace SCION_CORE::Scripting {

	void CreateVec2Bind(sol::state& lua)
	{
		auto vec2_multiply_overloads = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) { return v1 * v2; },
			[](const glm::vec2& v1, float v) { return v1 * v; },
			[](float v, const glm::vec2& v1) { return v * v1; }
		);

		auto vec2_divide_overloads = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) { return v1 / v2; },
			[](const glm::vec2& v1, float v) { return v1 / v; },
			[](float v, const glm::vec2& v1) { return v / v1; }
		);

		auto vec2_addition_overloads = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) { return v1 + v2; },
			[](const glm::vec2& v1, float v) { return v1 + v; },
			[](float v, const glm::vec2& v1) { return v + v1; }
		);

		auto vec2_subtraction_overloads = sol::overload(
			[](const glm::vec2& v1, const glm::vec2& v2) { return v1 - v2; },
			[](const glm::vec2& v1, float v) { return v1 - v; },
			[](float v, const glm::vec2& v1) { return v - v1; }
		);

		lua.new_usertype<glm::vec2>(
			"vec2",
			sol::call_constructor,
			sol::constructors<glm::vec2(float), glm::vec2(float, float)>(),
			"x", &glm::vec2::x,
			"y", &glm::vec2::y,
			sol::meta_function::multiplication, vec2_multiply_overloads,
			sol::meta_function::division, vec2_divide_overloads,
			sol::meta_function::addition, vec2_addition_overloads,
			sol::meta_function::subtraction, vec2_subtraction_overloads
		);
	}

	void CreateVec3Bind(sol::state& lua)
	{
		auto vec3_multiply_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) { return v1 * v2; },
			[](const glm::vec3& v1, float v) { return v1 * v; },
			[](float v, const glm::vec3& v1) { return v * v1; }
		);

		auto vec3_divide_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) { return v1 / v2; },
			[](const glm::vec3& v1, float v) { return v1 / v; },
			[](float v, const glm::vec3& v1) { return v / v1; }
		);

		auto vec3_addition_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) { return v1 + v2; },
			[](const glm::vec3& v1, float v) { return v1 + v; },
			[](float v, const glm::vec3& v1) { return v + v1; }
		);

		auto vec3_subtraction_overloads = sol::overload(
			[](const glm::vec3& v1, const glm::vec3& v2) { return v1 - v2; },
			[](const glm::vec3& v1, float v) { return v1 - v; },
			[](float v, const glm::vec3& v1) { return v - v1; }
		);

		lua.new_usertype<glm::vec3>(
			"vec3",
			sol::call_constructor,
			sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			sol::meta_function::multiplication, vec3_multiply_overloads,
			sol::meta_function::division, vec3_divide_overloads,
			sol::meta_function::addition, vec3_addition_overloads,
			sol::meta_function::subtraction, vec3_subtraction_overloads
		);
	}

	void CreateVec4Bind(sol::state& lua)
	{
		auto vec4_multiply_overloads = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) { return v1 * v2; },
			[](const glm::vec4& v1, float v) { return v1 * v; },
			[](float v, const glm::vec4& v1) { return v * v1; }
		);

		auto vec4_divide_overloads = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) { return v1 / v2; },
			[](const glm::vec4& v1, float v) { return v1 / v; },
			[](float v, const glm::vec4& v1) { return v / v1; }
		);

		auto vec4_addition_overloads = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) { return v1 + v2; },
			[](const glm::vec4& v1, float v) { return v1 + v; },
			[](float v, const glm::vec4& v1) { return v + v1; }
		);

		auto vec4_subtraction_overloads = sol::overload(
			[](const glm::vec4& v1, const glm::vec4& v2) { return v1 - v2; },
			[](const glm::vec4& v1, float v) { return v1 - v; },
			[](float v, const glm::vec4& v1) { return v - v1; }
		);

		lua.new_usertype<glm::vec4>(
			"vec4",
			sol::call_constructor,
			sol::constructors<glm::vec4(float), glm::vec4(float, float, float, float)>(),
			"x", &glm::vec4::x,
			"y", &glm::vec4::y,
			"z", &glm::vec4::z,
			"w", &glm::vec4::w,
			sol::meta_function::multiplication, vec4_multiply_overloads,
			sol::meta_function::division, vec4_divide_overloads,
			sol::meta_function::addition, vec4_addition_overloads,
			sol::meta_function::subtraction, vec4_subtraction_overloads
		);
	}

	void MathFreeFunctions(sol::state& lua)
	{
		lua.set_function("distance", sol::overload(
			[](const glm::vec2& a, const glm::vec2 b) { return glm::distance(a, b); },
			[](const glm::vec3& a, const glm::vec3 b) { return glm::distance(a, b); },
			[](const glm::vec4& a, const glm::vec4 b) { return glm::distance(a, b); }
		));

		lua.set_function("lerp", sol::overload(
			[](float a, float b, float t) { return std::lerp(a, b, t); }
		));

		lua.set_function("clamp", sol::overload(
			[](float v, float min, float max) { return std::clamp(v, min, max); },
			[](double v, double min, double max) { return std::clamp(v, min, max); },
			[](int v, int min, int max) { return std::clamp(v, min, max); }
		));
	}

	void CreateGLMBindings(sol::state& lua)
	{
		CreateVec2Bind(lua);
		CreateVec3Bind(lua);
		CreateVec4Bind(lua);

		MathFreeFunctions(lua);
	}

}