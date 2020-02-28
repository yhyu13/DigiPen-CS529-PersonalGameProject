#pragma once

#include <string>
#include <fstream>
#include "ComponentType.h"
#include "Common/MemoryPool.hpp"
/**
	Comment : 
	To create a custom component
	1. Add a new element to the enum type in the component type header
	2. Create a derived class which inherits from IComponent
	3. Create a derived class which inherits from IIComponentCreator
	4. RegisterSerialization in your initializer
	5. RegisterCreator in your initializer
*/
namespace My
{
	class ComponentManager;

	class IComponent
	{
	public:
		explicit IComponent(COMPONENT_TYPE type, void* owner, std::string name) noexcept;;
		virtual ~IComponent();
		
		virtual void Update(double deltaTime) = 0;
		virtual void Serialize(std::ifstream& fstream);
		virtual void Serialize(std::wostream& wostream) = 0;

		COMPONENT_TYPE GetType() noexcept;;
		
		void SetName(std::string name) noexcept;
		const std::string& GetName() const noexcept;
		void* GetOwner() const noexcept;;
		bool IsActive() noexcept;
		void SetActive(bool v) noexcept;

		friend ComponentManager;
	private:
		void SetOwner(void* owner) noexcept;;

	private:
		COMPONENT_TYPE m_type;
		std::string m_name;
		void* m_owner;
		bool m_active;
	};

	class IComponentCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IComponent* Create(void* owner, MemoryPool* pool, std::string name = "_com") = 0;
	};

	// TODO : Initialize in your application
	extern std::map<std::string, COMPONENT_TYPE> g_componentSerializationMap;
	extern std::map<COMPONENT_TYPE, std::shared_ptr<IComponentCreator>> g_componentCreatorMap;
}