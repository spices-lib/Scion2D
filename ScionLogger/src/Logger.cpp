#include "Logger.h"
#include <chrono>
#include <ctime>
#include <format>

namespace SCION_LOGGER {

	Logger::LogTime::LogTime(const std::string& data)
		: day{data.substr(0, 3)}
		, dayNumber{data.substr(8, 2)}
		, month{data.substr(4, 3)}
		, year{data.substr(20, 4)}
		, time{data.substr(11, 8)}
	{}

	std::string Logger::CurrentDataTime()
	{
		auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		char buf[30];

		ctime_s(buf, sizeof(buf), &time);

		LogTime logTime{ std::string(buf) };

		return std::format("{0}-{1}-{2} {3}", logTime.year, logTime.month, logTime.dayNumber, logTime.time);
	}

	Logger& Logger::GetInstance()
	{
		static Logger instance{};

		return instance;
	}

	void Logger::Init(bool consoleLog, bool retainLogs)
	{
		assert(!m_bInitialized && "Do not call Initialize more than once");

		m_bConsoleLog = consoleLog;
		m_bRetainLogs = retainLogs;
		m_bInitialized = true;
	}

}