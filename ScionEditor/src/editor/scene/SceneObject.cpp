#include "SceneObject.h"

namespace SCION_EDITOR {

	SceneObject::SceneObject(const std::string& sceneName)
		: m_Registry{}
		, m_RuntimeRegistry{}
		, m_sSceneName{ sceneName }
		, m_Canvas{}
	{

	}

	void SceneObject::CopySceneToRuntime()
	{

	}

	void SceneObject::ClearRuntimeScene()
	{

	}

	bool SceneObject::AddNewLayer()
	{
		static int number = { 0 };

		std::stringstream ss;
		ss << "NewLayer" << number;

		m_LayerParams.emplace_back(SCION_UTL::SpriteLayerParams{
			.sLayerName = ss.str()
		});

		number++;

		return true;
	}

}