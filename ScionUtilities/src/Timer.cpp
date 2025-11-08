#include "Timer.h"

namespace SCION_UTL {

	void Timer::Start()
	{
		if (!m_IsRunning)
		{
			m_StartPoint = std::chrono::steady_clock::now();
			m_IsRunning = true;
			m_bIsPause = false;
		}
	}

	void Timer::Stop()
	{
		if (m_IsRunning)
			m_IsRunning = false;
	}

	void Timer::Pause()
	{
		if (m_IsRunning && !m_bIsPause)
		{
			m_bIsPause = true;
			m_PausePoint = std::chrono::steady_clock::now();
		}
	}

	void Timer::Resume()
	{
		if (m_IsRunning && m_bIsPause)
		{
			m_bIsPause = false;
			m_StartPoint += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_PausePoint);
		}
	}

	const int64_t Timer::ElapsedMS()
	{
		if (m_IsRunning)
		{
			if (m_bIsPause)
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(m_PausePoint - m_StartPoint).count();
			}
			else
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_StartPoint).count();
			}
		}

		return 0;
	}

	const int64_t Timer::ELapsedSec()
	{
		return ElapsedMS() / 1000;
	}

}