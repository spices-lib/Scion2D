#include "StateMachine.h"
#include <Logger.h>

namespace SCION_CORE {

	StateMachine::StateMachine()
		: StateMachine{ sol::lua_nil_t{} }
	{}

	StateMachine::StateMachine(const sol::table& stateFuncs)
		: m_sCurrentState{ "" }
		, m_mapStates {}
		, m_StateTable{ stateFuncs }
	{}

	void StateMachine::ChangeState(const std::string& stateName, bool bRemoveState, const sol::object& enterParams)
	{
		/*if (m_StateTable)
		{
			ChangeStateTable(stateName, bRemoveState, enterParams);
			return;
		}*/

		if (!m_mapStates.contains(stateName))
		{
			SCION_ERROR("Failed to change states.");
			return;
		}

		auto& newState = m_mapStates.at(stateName);

		if (m_sCurrentState.empty())
		{
			m_sCurrentState = stateName;
		}
		else
		{
			auto& oldState = m_mapStates.at(m_sCurrentState);
			if (oldState->on_exit.valid())
			{
				try
				{
					auto result = oldState->on_exit();
					if (!result.valid())
					{
						sol::error error = result;
						throw error;
					}
				}
				catch (const sol::error& error)
				{
					SCION_ERROR("Failed to exit state: {}", error.what());
					return;
				}
			}

			if (bRemoveState)
			{
				oldState->bKillState;
			}

			m_sCurrentState = stateName;
		}

		if (newState->on_enter.valid())
		{
			try
			{
				auto result = newState->on_enter(enterParams);
				if (!result.valid())
				{
					sol::error error = result;
					throw error;
				}
			}
			catch (const sol::error& error)
			{
				SCION_ERROR("Failed to enter state: {}", error.what());
				return;
			}
		}
	}

	void StateMachine::Update(const float dt)
	{
		try
		{
			auto& state = m_mapStates.at(m_sCurrentState);

			auto result = state->on_update(dt);

			if (!result.valid())
			{
				sol::error error = result;
				throw error;
			}

			std::erase_if(m_mapStates, [](auto& state) {
				return state.second->bKillState;
			});
		}
		catch (const sol::error& error)
		{
			SCION_ERROR("Failed to update state: {}", error.what());
		}
		catch (...)
		{
			SCION_ERROR("Failed to update state");
		}
	}

	void StateMachine::Render()
	{
		try
		{
			auto& state = m_mapStates.at(m_sCurrentState);

			auto result = state->on_render();

			if (!result.valid())
			{
				sol::error error = result;
				throw error;
			}

			std::erase_if(m_mapStates, [](auto& state) {
				return state.second->bKillState;
			});
		}
		catch (const sol::error& error)
		{
			SCION_ERROR("Failed to render state: {}", error.what());
		}
		catch (...)
		{
			SCION_ERROR("Failed to render state");
		}
	}

	void StateMachine::AddState(const State& state)
	{
		if (m_mapStates.contains(state.name))
		{
			SCION_ERROR("Failed to add state: {}", state.name);
			return;
		}

		m_mapStates[state.name] = std::make_unique<State>(state);
	}

	void StateMachine::ExitState()
	{
		if (!m_mapStates.contains(m_sCurrentState))
		{
			SCION_ERROR("Failed to exit state: {}", m_sCurrentState);
			return;
		}

		auto& state = m_mapStates.at(m_sCurrentState);
		state->on_exit();
		state->bKillState = true;
		m_sCurrentState.clear();
	}

	void StateMachine::DestroyStates()
	{
		std::for_each(m_mapStates.begin(), m_mapStates.end(), [](const auto& pair) {
			pair.second->on_exit();
		});

		m_mapStates.clear();
	}

	void StateMachine::CreateLuaStateMachine(sol::state& lua)
	{
		lua.new_usertype<StateMachine>(
			"StateMachine",
			sol::call_constructor,
			sol::constructors<StateMachine(), StateMachine(const sol::table&)>(),
			"change_state",
			sol::overload(
				[](StateMachine& sm, const std::string& state, bool bRemove, const sol::object& enterParams) {
					sm.ChangeState(state, bRemove, enterParams);
				},
				[](StateMachine& sm, const std::string& state, bool bRemove) {
					sm.ChangeState(state, bRemove);
				},
				[](StateMachine& sm, const std::string& state) {
					sm.ChangeState(state);
				}
			),
			"update", &StateMachine::Update,
			"render", &StateMachine::Render,
			"current_state", &StateMachine::CurrentState,
			"add_state", &StateMachine::AddState,
			"exit_state", &StateMachine::ExitState,
			"destroyStates", &StateMachine::DestroyStates
		);
	}
}