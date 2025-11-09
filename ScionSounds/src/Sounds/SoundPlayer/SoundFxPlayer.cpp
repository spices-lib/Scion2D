#include "SoundFxPlayer.h"
#include "Sounds/Essentials/SoundFX.h"
#include <Logger.h>

namespace SCION_SOUNDS {

	void SoundFxPlayer::Play(SoundFX& soundFx)
	{
		Play(soundFx, 0, -1);
	}

	void SoundFxPlayer::Play(SoundFX& soundFx, int loops, int channel)
	{
		if (!soundFx.GetSoundFxPtr())
		{
			SCION_ERROR("Failed to play soundFx {}", soundFx.GetName());
			return;
		}

		if (Mix_PlayChannel(channel, soundFx.GetSoundFxPtr(), loops) == -1)
		{
			std::string error{Mix_GetError()};
			SCION_ERROR("Failed to play soundFx {}", error);
			return;
		}
	}
	void SoundFxPlayer::SetVolume(int channel, int volume)
	{
		if (volume > 100)
		{
			volume = 100;
		}

		if (volume < 0)
		{
			volume = 0;
		}

		int volume_changed = static_cast<int>((volume / 100.0f) * 128);
		Mix_VolumeMusic(volume_changed);
	}

	void SoundFxPlayer::Stop(int channel)
	{
		if (Mix_HaltChannel(channel) == -1)
		{
			std::string error{Mix_GetError()};
			SCION_ERROR("Failed to halt soundFx {}", error);
		}
	}

	bool SoundFxPlayer::IsPlaying(int channel)
	{
		return Mix_Playing(channel);
	}
}