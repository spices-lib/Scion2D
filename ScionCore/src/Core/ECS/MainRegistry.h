#pragma once
#include "Registry.h"
#include <memory>

namespace SCION_RESOURCE {

	class AssetManager;
}

namespace SCION_SOUNDS {

	class MusicPlayer;
	class SoundFxPlayer;
}

namespace SCION_CORE::ECS {

	class MainRegistry
	{
	private:

		std::unique_ptr<Registry> m_pMainRegistry{ nullptr };
		bool m_bInitialized{ false };

		MainRegistry() = default;
		~MainRegistry() = default;
		MainRegistry(const MainRegistry&) = delete;
		MainRegistry& operator=(const MainRegistry&) = delete;

	public:

		static MainRegistry& GetInstance()
		{
			static MainRegistry instance;
			return instance;
		}

		void Initialize();

		SCION_RESOURCE::AssetManager& GetAssetManager()
		{
			return *GetContext<std::shared_ptr<SCION_RESOURCE::AssetManager>>();
		}
		SCION_SOUNDS::MusicPlayer& GetMusicPlayer()
		{
			return *GetContext<std::shared_ptr<SCION_SOUNDS::MusicPlayer>>();
		}
		SCION_SOUNDS::SoundFxPlayer& GetSoundPlayer()
		{
			return *GetContext<std::shared_ptr<SCION_SOUNDS::SoundFxPlayer>>();
		}

		template<typename TContext>
		TContext AddToContext(TContext context)
		{
			return m_pMainRegistry->AddToContext(context);
		}

		template<typename TContext>
		TContext GetContext()
		{
			return m_pMainRegistry->GetContext<TContext>();
		}

	};
}