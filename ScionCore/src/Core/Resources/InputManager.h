#pragma once
#include <Windowing/Inputs/Keyboard.h>
#include <Windowing/Inputs/Mouse.h>
#include <Windowing/Inputs/Gamepad.h>
#include <memory>
#include <sol/sol.hpp>

using namespace SCION_WINDOWING::Inputs;

namespace SCION_CORE {

	constexpr int MAX_CONTROLLERS = 4;

	class InputManager
	{
	private:
		std::unique_ptr<Keyboard> m_pKeyboard;
		std::unique_ptr<Mouse> m_pMouse;
		std::map<int, std::shared_ptr<Gamepad>> m_mapGameControllers;

	private:

		InputManager();
		~InputManager() = default;
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;

	private:

		static void RegisterLuaKeyNames(sol::state& lua);
		static void RegisterLuaMouseButtonNames(sol::state& lua);
		static void RegisterLuaGamepadButtonNames(sol::state& lua);

	public:

		static InputManager& GetInstance();
		static void CreateLuaInputBindings(sol::state& lua);

		inline Keyboard& GetKeyboard() { return *m_pKeyboard; }
		inline Mouse& GetMouse() { return *m_pMouse; }

		std::shared_ptr<Gamepad> GetController(int index);

		int AddGamepad(Sint32 gamepadIndex);
		int RemoveGamepad(Sint32 gamepadID);
		void GamepadButtonPressed(const SDL_Event& event);
		void GamepadButtonReleased(const SDL_Event& event);
		void GamepadAxisValues(const SDL_Event& event);
		void GamepadHatValues(const SDL_Event& event);
		void UpdateGamepads();
	};
}