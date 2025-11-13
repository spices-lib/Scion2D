#include "State.h"
#include <Logger.h>

namespace SCION_CORE {

	void State::CreateLuaStateBind(sol::state& lua)
	{
		lua.new_usertype<State>(
			"State",
			sol::call_constructor,
			sol::factories(
				[](const std::string& name) {
					return State{ .name = name };
				},
				[](const std::string& name, sol::protected_function on_enter, sol::protected_function on_exit, 
					sol::protected_function on_update, sol::protected_function on_render, sol::protected_function handle_inputs, 
					sol::object variables) {
						return State{
							.name = name,
							.on_render = on_render,
							.on_update = on_update,
							.on_enter = on_enter,
							.on_exit = on_exit,
							.handle_inputs = handle_inputs,
							.variables = variables
						};
				}
			),
			"set_on_enter", [](State& state, sol::protected_function on_enter) {
				if (!on_enter.valid())
				{
					SCION_ERROR("Failed to set function");
					return;
				}
				state.on_enter = on_enter;
			},
			"set_on_exit", [](State& state, sol::protected_function on_exit) {
				if (!on_exit.valid())
				{
					SCION_ERROR("Failed to set function");
					return;
				}
				state.on_exit = on_exit;
			},
			"set_on_update", [](State& state, sol::protected_function on_update) {
				if (!on_update.valid())
				{
					SCION_ERROR("Failed to set function");
					return;
				}
				state.on_update = on_update;
			},
			"set_on_render", [](State& state, sol::protected_function on_render) {
				if (!on_render.valid())
				{
					SCION_ERROR("Failed to set function");
					return;
				}
				state.on_render = on_render;
			},
			"set_handle_inputs", [](State& state, sol::protected_function handle_inputs) {
				if (!handle_inputs.valid())
				{
					SCION_ERROR("Failed to set function");
					return;
				}
				state.handle_inputs = handle_inputs;
			},
			"set_variables_table", [](State& state, const sol::table& table) {
				if (!table.valid())
				{
					SCION_ERROR("Failed to set function");
					return;
				}
				state.variables = table;
			},
			"variables", &State::variables,
			"name", &State::name,
			"bKillState", &State::bKillState
		);
	}
}