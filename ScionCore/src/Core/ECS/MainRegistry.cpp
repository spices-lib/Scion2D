#include "MainRegistry.h"
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Sounds/SoundPlayer/SoundFxPlayer.h>

namespace SCION_CORE::ECS {

	void MainRegistry::Initialize()
	{

	}

	SCION_RESOURCES::AssetManager& MainRegistry::GetAssetManager()
	{
		return *GetContext<std::shared_ptr<SCION_RESOURCES::AssetManager>>();
	}

	SCION_SOUNDS::MusicPlayer& MainRegistry::GetMusicPlayer()
	{
		return *GetContext<std::shared_ptr<SCION_SOUNDS::MusicPlayer>>();
	}

	SCION_SOUNDS::SoundFxPlayer& MainRegistry::GetSoundPlayer()
	{
		return *GetContext<std::shared_ptr<SCION_SOUNDS::SoundFxPlayer>>();
	}
}