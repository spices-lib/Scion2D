#pragma once
#include "IDisplay.h"
#include <string>
#include <ScionUtilities.h>

namespace SCION_UTL {
	enum class AssetType;
}

namespace SCION_EDITOR {

	class AssetDisplay : public IDisplay
	{
	private:

		const std::vector<std::string> m_SelectedTypes{ "TEXTURES", "FONTS", "MUSIC", "SOUNDFX", "SCENE" };
		bool m_bItemHovered, m_bAsserTypeChanged, m_bRename;
		bool m_bWindowSelected, m_bWindowHovered;
		std::string m_sSelectedAssetName, m_sSelectedType, m_sDragSource, m_sRenameBuf;
		SCION_UTL::AssetType m_eSelectedType;
		int m_SelectedID;

		float m_AssetSize;

	private:

		void SetAssetType();
		void DrawSelectedAssets();
		unsigned int GetTextureID(const std::string& sAssetName);
		bool DoRenameAsset(const std::string& sOldName, const std::string& sNewName);
		void CheckRename(const std::string& name);
		void OpenAssetContext(const std::string& sAssetName);

	public:

		AssetDisplay();
		~AssetDisplay() override = default;

		virtual void Draw() override;
		virtual void Update() override;
	};

}