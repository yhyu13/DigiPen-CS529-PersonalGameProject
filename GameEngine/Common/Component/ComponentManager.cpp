#include "ComponentManager.hpp"

My::ComponentManager::ComponentManager() noexcept
	:
	m_owner(nullptr)
{
}

int My::ComponentManager::Initialize()
{
	return 0;
}

int My::ComponentManager::Tick(double deltaTime)
{
#if MEMORY_POOL_OPT
	for (auto& com : m_pool.GetBookKeeper()) 
	{ 
		reinterpret_cast<IComponent*>(com)->Update(deltaTime); 
	}
#else
	for (auto& com : m_components) 
	{ 
		com->Update(deltaTime);
	}
#endif
	return 0;
}

int My::ComponentManager::Finalize()
{
	return 0;
}

void My::ComponentManager::Serialize(std::wostream& wostream)
{
#if MEMORY_POOL_OPT
	for (auto& com : m_pool.GetBookKeeper())
	{
		reinterpret_cast<IComponent*>(com)->Serialize(wostream);
	}
#else
	for (auto& com : m_components)
	{
		com->->Serialize(wostream);
	}
#endif
}

/**
 *	\brief AddComponent: add newly allocated component to the component manager.
 *
 *	Set the ownership of that component to this component manager.
 *  \author Hang Yu
 *	\date 10/21/2019
 *	\return void.
 *	\exception None.
 *  @param com IComponent pointer.
 */
My::IComponent* My::ComponentManager::AddComponent(COMPONENT_TYPE type)
{
	IComponent* com;
	if (com = GetComponent(type))
	{
		return com;
	}
#if MEMORY_POOL_OPT
	com = g_componentCreatorMap[type]->Create(this,&m_pool);
#else
	com = g_componentCreatorMap[type]->Create(this);
	m_components.push_back(std::shared_ptr<IComponent>(com));	
#endif
	return com;
}

/**
 *	\brief GetComponent: return component with a specified type
 *  \author Hang Yu
 *	\date 10/21/2019
 *	\return nullptr if such type does not exist.
 *	\exception None.
 *  @param type COMPONENT_TYPE enumerator.
 */
My::IComponent* My::ComponentManager::GetComponent(COMPONENT_TYPE type)
{
#if MEMORY_POOL_OPT
	for (auto com : m_pool.GetBookKeeper())
	{
		if (com == nullptr)
		{
			return nullptr;
		}
		else if (reinterpret_cast<IComponent*>(com)->GetType() == type)
		{
			return reinterpret_cast<IComponent*>(com);
		}
	}
#else
	for (auto com : m_components)
	{
		if (com == nullptr)
		{
			return nullptr;
		}
		else if (com->GetType() == type)
		{
			return com.get();
		}
	}
#endif
	return nullptr;
}

void* My::ComponentManager::GetOwner() noexcept 
{ 
	return m_owner; 
}

void  My::ComponentManager::SetOwner(void* owner) noexcept 
{ 
	m_owner = owner; 
}