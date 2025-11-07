#pragma once

namespace SCION_WINDOWING::Inputs {

	struct Button
	{
		bool bIsPressed{ false }, bJustPressed{ false }, bJusyReleased{ false };

		void Update(bool bPressed);
		void Reset();

	};
}