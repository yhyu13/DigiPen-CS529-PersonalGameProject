#pragma once

#include "IGameObject.hpp"

namespace My
{
	class IGameObjectResourceManager : public ResourceManager<IGameObject>
	{
	public:
		/* Update all game objects, and erase inactive game objects afterwards */
		virtual int Tick(double deltaTime) override;
		int DrawAll();
		void GC();

		/* Use Memory Manager's Delete function to free all pointers managed by this resource manager.- */
		virtual int Clear() override
		{
			for (auto it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it)
			{
				g_pMemoryManager->Delete(it->second);
			}
			m_resourceMap.clear();
			return 0;
		}

		/* Find all resources that contains name */
		std::vector<IGameObject*> FindAllResourcesByName(std::string sub_name)
		{
			std::vector<IGameObject*> result;
			for (auto it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it)
			{
				if (it->first.find(sub_name) != std::string::npos)
				{
					result.push_back(it->second);
				}
			}
			return result;
		}
	private:
		std::vector<std::string> gc_list;
	};
	// TODO : Initialize in your application
	extern std::shared_ptr<IGameObjectResourceManager> g_pGameObjectResourceManager;

	// TODO : Initialize in your application
	extern std::map<std::string, GAMEOBJECT_TYPE> g_gameObjectSerializationMap;
	extern std::map<GAMEOBJECT_TYPE, std::shared_ptr<IGameObjectCreator>> g_gameObjectCreatorMap;
}