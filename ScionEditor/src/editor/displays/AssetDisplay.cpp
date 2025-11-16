#include "AssetDisplay.h"
#include <imgui.h>
#include <Core/ECS/MainRegistry.h>
#include <Logger.h>
#include <Core/Resources/InputManager.h>
#include <Core/Resources/AssetManager.h>
#include "editor/utilities/EditorUtilities.h"
#include "editor/scene/SceneManager.h"

constexpr float DEFAULT_ASSET_SIZE = 128.0f;
constexpr ImVec2 DRAG_ASSET_SIZE = ImVec2{ 32.0f, 32.0f };

namespace SCION_EDITOR {

	void AssetDisplay::SetAssetType()
	{
		if (!m_bAsserTypeChanged)
			return;

		if (m_sSelectedType == "TEXTURES")
		{
			m_eSelectedType = SCION_UTL::AssetType::TEXTURE;
			m_sDragSource = DROP_TEXTURE_SRC;
		}
		else if (m_sSelectedType == "FONTS")
		{
			m_eSelectedType = SCION_UTL::AssetType::FONT;
			m_sDragSource = DROP_FONT_SRC;
		}
		else if (m_sSelectedType == "SOUNDFX")
		{
			m_eSelectedType = SCION_UTL::AssetType::SOUNDFX;
			m_sDragSource = DROP_SOUNDFX_SRC;
		}
		else if (m_sSelectedType == "MUSIC")
		{
			m_eSelectedType = SCION_UTL::AssetType::MUSIC;
			m_sDragSource = DROP_MUSIC_SRC;
		}
		else if (m_sSelectedType == "SCENES")
		{
			m_eSelectedType = SCION_UTL::AssetType::SCENE;
			m_sDragSource = DROP_SCENE_SRC;
		}
		else
		{
			m_eSelectedType = SCION_UTL::AssetType::NO_TYPE;
			m_sDragSource = "NO_ASSET_TYPE";
		}

		m_bAsserTypeChanged;
	}

	void AssetDisplay::DrawSelectedAssets()
	{
		auto& mainRegistry = SCION_CORE::ECS::MainRegistry::GetInstance();
		auto& assetManager = mainRegistry.GetAssetManager();

		std::vector<std::string> assetNames;

		if (m_eSelectedType == SCION_UTL::AssetType::SCENE)
		{
			assetNames = SceneManager::GetInstance().GetSceneNames();
		}
		else
		{
			assetNames = assetManager.GetAssetKeyNames(m_eSelectedType);
		}

		if (assetNames.empty())
			return;

		const auto& windowWidth = ImGui::GetWindowWidth();

		int numCols = static_cast<int>((windowWidth - m_AssetSize) / m_AssetSize);
		int numRows = static_cast<int>(assetNames.size() / (numCols <= 1 ? 1 : numCols) + 1);

		if (!numCols || !numRows)
			return;

		ImGuiTableFlags tableFlags{ 0 };
		tableFlags |= ImGuiTableFlags_SizingFixedFit;

		int k{ 0 }, id{ 0 };

		auto assetItr = assetNames.begin();

		if (ImGui::BeginTable("Assets", numCols, tableFlags))
		{
			for (int row = 0; row < numRows; row++)
			{
				ImGui::TableNextRow();
				for (int col = 0; col < numCols; col++)
				{
					if (assetItr == assetNames.end())
						break;

					ImGui::TableSetColumnIndex(col);

					ImGui::PushID(k++);
					ImGui::TableSetColumnIndex(col);

					bool bSelectedAsset{ m_SelectedID == id };

					if (bSelectedAsset)
					{
						ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg,
							ImGui::GetColorU32(ImVec4{ 0.0f, 0.9f, 0.0f, 0.3f }));
					}

					GLuint textureID{ GetTextureID(*assetItr) };
					std::string sCheckName{ m_sRenameBuf.data() };

					if (textureID == 0)
						break;

					ImGui::ImageButton("##", textureID, ImVec2{m_AssetSize, m_AssetSize});

					if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0) && !m_bRename)
						m_SelectedID = id;

					auto sAssetName = (*assetItr).c_str();
					if (bSelectedAsset && ImGui::BeginPopupContextItem())
					{
						OpenAssetContext(*assetItr);
						ImGui::EndPopup();
					}

					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload(m_sDragSource.c_str(), sAssetName, (strlen(sAssetName) + 1) * sizeof(char), ImGuiCond_Once);

						ImGui::Image(textureID, DRAG_ASSET_SIZE);
						ImGui::EndDragDropSource();
					}

					if (!m_bRename || !bSelectedAsset)
						ImGui::Text(sAssetName);

					if (m_bRename && bSelectedAsset)
					{
						ImGui::SetKeyboardFocusHere();
						if (ImGui::InputText("##rename", m_sRenameBuf.data(), 255, ImGuiInputTextFlags_EnterReturnsTrue))
						{
							if (!DoRenameAsset(*assetItr, sCheckName))
							{
								SCION_ERROR("Failed to change asset name.");
							}

							m_sRenameBuf.clear();
							m_bRename = false;
						}
						else if (m_bRename && ImGui::IsKeyPressed(ImGuiKey_Escape))
						{
							m_sRenameBuf.clear();
							m_bRename = false;
						}
					}

					if (!m_bRename && bSelectedAsset && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
					{
						m_sRenameBuf.clear();
						m_sRenameBuf = *assetItr;
						m_bRename = true;
					}

					if (m_bRename && bSelectedAsset)
					{
						if (sAssetName != sCheckName)
							CheckRename(sCheckName);
					}

					++id;
					++assetItr;
					ImGui::PopID();
				}
			}

			ImGui::EndTable();
		}
	}

	unsigned int AssetDisplay::GetTextureID(const std::string& sAssetName)
	{
		auto& assetManager = SCION_CORE::ECS::MainRegistry::GetInstance().GetAssetManager();

		switch (m_eSelectedType)
		{
			case SCION_UTL::AssetType::TEXTURE: {
				auto& pTexture = assetManager.GetTexture(sAssetName);
				return pTexture.GetID();
			}
			case SCION_UTL::AssetType::FONT: {
				auto pTexture = assetManager.GetFont(sAssetName);
				return pTexture->GetFontAtlasID();
			}
			case SCION_UTL::AssetType::SOUNDFX: {
				break;
			}
			case SCION_UTL::AssetType::MUSIC: {
				auto& pTexture = assetManager.GetTexture("music_icon");
				return pTexture.GetID();
			}
			case SCION_UTL::AssetType::SCENE: {
				auto& pTexture = assetManager.GetTexture("scene_icon");
				return pTexture.GetID();
			}
		}

		return 0;
	}

	bool AssetDisplay::DoRenameAsset(const std::string& sOldName, const std::string& sNewName)
	{
		if (sNewName.empty())
			return false;

		if (m_eSelectedType == SCION_UTL::AssetType::SCENE)
		{

		}
		else
		{
			auto& assetManaget = SCION_CORE::ECS::MainRegistry::GetInstance().GetAssetManager();
			return assetManaget.ChangeAssetName(sOldName, sNewName, m_eSelectedType);
		}

		return false;
	}

	void AssetDisplay::CheckRename(const std::string& name)
	{
		if (name.empty())
		{
			ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, "Rename text cannot be blank!");
			return;
		}

		bool bHasAsset{ false };
		if (m_eSelectedType == SCION_UTL::AssetType::SCENE)
		{

		}
		else
		{
			auto& assetManager = SCION_CORE::ECS::MainRegistry::GetInstance().GetAssetManager();
			bHasAsset = assetManager.CheckHasAsset(name, m_eSelectedType);
		}

		if (bHasAsset)
		{
			ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, "Asset name exist!");
		}
	}

	void AssetDisplay::OpenAssetContext(const std::string& sAssetName)
	{
		if (ImGui::Selectable("name"))
		{
			m_bRename = true;
		}

		if (ImGui::Selectable("delete"))
		{
			auto& assetManager = SCION_CORE::ECS::MainRegistry::GetInstance().GetAssetManager();
			if (!assetManager.DeleteAsset(sAssetName, m_eSelectedType))
			{
				SCION_ERROR("Failed to delete asset.");
			}
		}
	}

	AssetDisplay::AssetDisplay()
		: m_bItemHovered{ false }
		, m_bAsserTypeChanged{ true }
		, m_bRename{ false }
	    , m_bWindowSelected{ false }
		, m_bWindowHovered{ false }
		, m_sSelectedAssetName{ "" }
		, m_sSelectedType{ "TEXTURES" }
		, m_sDragSource{ "" }
		, m_sRenameBuf{ "" }
		, m_eSelectedType{ SCION_UTL::AssetType::TEXTURE }
		, m_SelectedID{ 0 }
	{
		SetAssetType();
	}

	void AssetDisplay::Draw()
	{
		if (!ImGui::Begin("Asset"))
		{
			ImGui::End();
			return;
		}

		auto& mainRegistry = SCION_CORE::ECS::MainRegistry::GetInstance();
		auto& assetManager = mainRegistry.GetAssetManager();

		ImGui::Text("ASSET TYPE");
		ImGui::SameLine(0.0f, 10.0f);
		if (ImGui::BeginCombo("##AssetType", m_sSelectedType.c_str()))
		{
			for (const auto& sAssetType : m_SelectedTypes)
			{
				bool bIsSelected = sAssetType == m_sSelectedType;
				if (ImGui::Selectable(sAssetType.c_str(), bIsSelected))
				{
					m_bAsserTypeChanged = true;
					m_sSelectedType = sAssetType;
					m_SelectedID = -1;
					SetAssetType();
				}

				if (bIsSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		if (ImGui::BeginChild("##AssetTable", ImVec2{ 0.0f, 0.0f }, NULL, ImGuiWindowFlags_AlwaysVerticalScrollbar |
			ImGuiWindowFlags_ChildWindow))
		{
			m_bWindowHovered = ImGui::IsWindowHovered();
			m_bWindowSelected = ImGui::IsWindowFocused();

			DrawSelectedAssets();
		}

		ImGui::End();
	}

	void AssetDisplay::Update()
	{

	}

}