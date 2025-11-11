#pragma once

namespace SCION_CORE {
	
	class CoreEngineData
	{
	public:

		static CoreEngineData& GetInstance()
		{
			static CoreEngineData data;
			return data;
		}

		int WindowWidth() { return m_WindowWidth; }
		int WindowHeight() { return m_WindowHeight; }

	private:

		int m_WindowWidth = 480;
		int m_WindowHeight = 640;

	};

}