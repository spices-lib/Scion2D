#include "SoundFX.h"

namespace SCION_SOUNDS {

	SoundFX::SoundFX(const SoundParams& params, SoundFxPtr pSoundFx)
		: m_Params{ params }
		, m_pSoundFx{ std::move(pSoundFx) }
	{}
}