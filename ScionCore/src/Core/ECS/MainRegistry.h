#pragma once
#include "Registry.h"
#include <memory>

namespace SCION_RESOURCES {

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

		SCION_RESOURCES::AssetManager& GetAssetManager();
		SCION_SOUNDS::MusicPlayer& GetMusicPlayer();
		SCION_SOUNDS::SoundFxPlayer& GetSoundPlayer();

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