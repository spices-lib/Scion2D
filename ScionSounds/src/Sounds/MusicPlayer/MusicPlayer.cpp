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
	}
	void MusicPlayer::Resume()
	{
	}
	void MusicPlayer::Stop()
	{
	}
	void MusicPlayer::SetVolume(int volume)
	{
	}
	void MusicPlayer::IsPlaying()
	{
	}
}