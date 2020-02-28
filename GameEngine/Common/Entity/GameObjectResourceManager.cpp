#include "GameObjectResourceManager.hpp"
#include "Common/MemoryManager.hpp"
#include "Common/Component/Physics/BodyResourceManager.hpp"
#include "Common/Utility/Utility.hpp"

int My::IGameObjectResourceManager::Tick(double deltaTime)
{
	for (auto it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it)
	{
		if (it->second->IsActive())
		{
			it->second->Tick(deltaTime);
		}
		else if (it->second->IsGC())
		{
			gc_list.push_back(it->first);
		}
	}
	return 0;
}
int My::IGameObjectResourceManager::DrawAll()
{
	for (auto it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it)
	{
		if (it->second->IsActive())
		{
			it->second->Draw();
		}
	}
	return 0;
}

void My::IGameObjectResourceManager::GC()
{
	// GC
	for (auto& name : gc_list)
	{
		g_pBodyResourceManager->FreeByNameWithDefault(name);
		FreeByNameWithMemoryManager(name);
		DEBUG_PRINT("Object GC " + name + "\n");
	}
	gc_list.clear();
}
