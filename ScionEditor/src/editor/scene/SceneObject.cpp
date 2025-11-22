#include "SceneObject.h"
#include <Core/ECS/Components/ComponentSerializer.h>
#include <Core/ECS/MetaUtilities.h>

using namespace SCION_CORE::ECS;

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
		auto& registryToCopy = m_Registry.GetRegistry();
		auto& runtimeRegistry = m_RuntimeRegistry.GetRegistry();

		for (auto entityToCopy : registryToCopy.view<entt::entity>(entt::exclude<ScriptComponent>))
		{
			entt::entity newEntity = m_RuntimeRegistry.CreateEntity();

			for (auto&& [id, storage] : registryToCopy.storage())
			{
				if (!storage.contains(entityToCopy))
					continue;

				//SCION_CORE::Utils::InvokeMetaFunction(id, "copy_component"_hs, Entity{ m_Registry, entityToCopy }, Entity{m_RuntimeRegistry, newEntity});
			}
		}
	}

	void SceneObject::ClearRuntimeScene()
	{
		m_RuntimeRegistry.GetRegistry().clear();
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