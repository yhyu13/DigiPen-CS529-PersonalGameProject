#pragma once
#include "Common/Interface/IRunTimeModule.hpp"
#include "Common/Component/Interface/IComponent.hpp"
#include "Common/MemoryPool.hpp"

#define MEMORY_POOL_OPT 1

namespace My
{
	class ComponentManager : public IRunTimeModule
	{
	public:

		ComponentManager() noexcept;
		ComponentManager(const ComponentManager&) = delete;
		ComponentManager& operator=(const ComponentManager&) = delete;

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;
		virtual void Serialize(std::wostream& wostream) override;

		/**
		 *	\brief AddComponent: add newly allocated component to the component manager of such type. 
		 *
		 *  If the component of such type already exists, simply return the existed component.
		 *	Set the ownership of that component to this component manager.
		 *  \author Hang Yu
		 *	\date 10/21/2019
		 *	\return pointer to component.
		 *	\exception None.
		 *  @param com IComponent pointer.
		 */
		IComponent* AddComponent(COMPONENT_TYPE type);

		/**
		 *	\brief GetComponent: return component with such type
		 *  \author Hang Yu
		 *	\date 10/21/2019
		 *	\return pointer to component pointer, nullptr if such component type does not exist.
		 *	\exception None.
		 *  @param type COMPONENT_TYPE enumerator.
		 */
		IComponent* GetComponent(COMPONENT_TYPE type);

		void* GetOwner() noexcept;
		void SetOwner(void* owner) noexcept;

	private:
		// Owner is an entity (e.g. IGameObject)
		void* m_owner;

#if MEMORY_POOL_OPT
		MemoryPool m_pool;
#else
		std::vector<std::shared_ptr<IComponent>> m_components;
#endif // MEMORY_POOL_OPT		
	};
}