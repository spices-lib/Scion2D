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

		float PixelsToMeters() { return 1.0f; }
		float ScaledHeight() { return 1.0f; }
		float ScaledWidth() { return 1.0f; }

	private:

		int m_WindowWidth = 480;
		int m_WindowHeight = 640;

	};

}