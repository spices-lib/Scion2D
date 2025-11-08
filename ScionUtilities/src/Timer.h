#pragma once
#include <chrono>

namespace SCION_UTL {

	class Timer
	{
	private:

		std::chrono::time_point<std::chrono::steady_clock> m_StartPoint, m_PausePoint;
		bool m_IsRunning{ false }, m_bIsPause{ false };

	public:
		Timer() = default;
		~Timer() = default;

		void Start();
		void Stop();
		void Pause();
		void Resume();

		const int64_t ElapsedMS();
		const int64_t ELapsedSec();

		inline const bool IsRunning() const { return m_IsRunning; }
		inline const bool IsPause() const { return m_bIsPause; }

	};
}