#include "SoundBinding.h"
#include "Core/ECS/Registry.h"
#include "Core/Resources/AssetManager.h"
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Logger.h>

using namespace SCION_SOUNDS;
using namespace SCION_RESOURCE;

namespace SCION_CORE::Scripting {

	void SoundBinder::CreateSoundBind(sol::state& lua, SCION_CORE::ECS::Registry& registry)
	{
		auto& musicPlayer = registry.GetContext<std::shared_ptr<MusicPlayer>>();
		if (!musicPlayer)
		{
			SCION_ERROR("Failed to bind music player to lua");
			return;
		}

		auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();

		lua.new_usertype<MusicPlayer>(
			"Music",
			sol::no_constructor,
			"play", sol::overload(
				[&](const std::string& musicName, int loops) {
					auto music = assetManager->GetMusic(musicName);
					if (!music)
					{
						SCION_ERROR("Failed to get music {0}", musicName);
						return;
					}
					musicPlayer->Play(*music, loops);
				},
				[&](const std::string& musicName) {
					auto music = assetManager->GetMusic(musicName);
					if (!music)
					{
						SCION_ERROR("Failed to get music {0}", musicName);
						return;
					}
					musicPlayer->Play(*music, -1);
				}
			),
			"stop", [&]() {
				musicPlayer->Stop();
			},
			"pause", [&]() {
				musicPlayer->Pause();
			},
			"resume", [&]() {
				musicPlayer->Resume();
			},
			"set_volume", [&](int volume) {
				musicPlayer->SetVolume(volume);
			},
			"is_playing", [&]() {
				return musicPlayer->IsPlaying();
			}
		);
	}

}