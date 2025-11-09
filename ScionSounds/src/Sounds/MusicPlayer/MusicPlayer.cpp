#include "MusicPlayer.h"
#include "Sounds/Essentials/Music.h"
#include <Logger.h>

namespace SCION_SOUNDS {

	MusicPlayer::MusicPlayer()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			std::string error{ Mix_GetError() };
			SCION_ERROR("Unable to open SDL Music Mixer - {}", error);
			return;
		}

		SCION_LOG("CHANNEDL ALLOCATE {}", Mix_AllocateChannels(16));
	}

	MusicPlayer::~MusicPlayer()
	{
		Mix_HaltMusic();
		Mix_Quit();
		SCION_LOG("Music Player Colsed!");
	}

	void MusicPlayer::Play(Music& music, int loops)
	{
		if (!music.GetMusicPtr())
		{
			SCION_ERROR("Failed to play music {}", music.GetName());
			return;
		}

		if (Mix_PlayMusic(music.GetMusicPtr(), loops) != 0)
		{
			std::string error{ Mix_GetError() };
			SCION_ERROR("Failed to play music {}", error);
			return;
		}
	}

	void MusicPlayer::Pause()
	{
		Mix_PauseMusic();
	}

	void MusicPlayer::Resume()
	{
		Mix_ResumeMusic();
	}

	void MusicPlayer::Stop()
	{
		Mix_HaltMusic();
	}

	void MusicPlayer::SetVolume(int volume)
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

	bool MusicPlayer::IsPlaying()
	{
		return Mix_PlayingMusic();
	}

}