#include "SceneManager.h"
#include <Logger.h>
#include "SceneObject.h"
#include <ScionUtilities.h>

namespace SCION_EDITOR {

	SceneManager& SceneManager::GetInstance()
	{
		static SceneManager instance;

		return instance;
	}

	bool SceneManager::AddScene(const std::string& sSceneName)
	{
		if (m_mapScenes.contains(sSceneName))
		{
			SCION_ERROR("Failed to add scene");
			return false;
		}

		m_mapScenes[sSceneName] = std::make_shared<SCION_EDITOR::SceneObject>(sSceneName);
		return true;
	}

	std::shared_ptr<SCION_EDITOR::SceneObject> SceneManager::GetScene(const std::string& sSceneName)
	{
		if (!m_mapScenes.contains(sSceneName))
		{
			return nullptr;
		}

		return m_mapScenes.at(sSceneName);
	}

	std::shared_ptr<SCION_EDITOR::SceneObject> SceneManager::GetCurrentScene()
	{
		if (!m_mapScenes.contains(m_sCurrentScene))
		{
			return nullptr;
		}

		return m_mapScenes.at(m_sCurrentScene);
	}

	std::vector<std::string> SceneManager::GetSceneNames()
	{
		return SCION_UTL::GetKeys(m_mapScenes);
	}

}