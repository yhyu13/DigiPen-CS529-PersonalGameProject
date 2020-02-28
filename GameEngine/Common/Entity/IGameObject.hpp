#pragma once
#include "GameObjectType.h"
#include "Common/Component/ComponentManager.hpp"
#include "Common/MemoryManager.hpp"
#include "Common/ResourceManager.hpp"
#include "Common/Event/EventManager.hpp"

/**
	Comment : 
	To create a custom game object
	1. Add a new element to the enum type in the game object type header
	2. Create a derived class which inherits from IGameObject
	3. Create a derived class which inherits from IGameObjectCreator
	4. RegisterSerialization in your initializer
	5. RegisterCreator in your initializer
*/
namespace My
{
	/**
	*	IGameObject is a combination of these two interfaces: IRunTimeModule, IComponent but only inherits from IRunTimeModule.
	*/
	class IGameObject : public IRunTimeModule
	{
	public:
		IGameObject(const IGameObject&) = delete;
		IGameObject& operator=(const IGameObject&) = delete;
		explicit IGameObject(GAMEOBJECT_TYPE type, void* owner, std::string name) noexcept;
		// Virtual destructor is needed since we will use IGameObject* (the base class pointer)
		virtual ~IGameObject() {};
		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;
		virtual int Draw();
		virtual void Serialize(std::wostream& wostream) override;
		
		std::vector<IGameObject*>& GetChildren();
		void AddChildren(IGameObject* child);	

		std::map<std::string, std::function<void(void)>>& GetCallBacks();
		void AddCallBacks(std::string name, std::function<void(void)> func);

		/* Queued process of a event*/
		void AddEvent(std::weak_ptr<Event> e);		
		/* Immediate process of a event */
		virtual void HandleEvent(std::weak_ptr<Event> e) {}; 

		/* Create a new component of specific type and add to the game object */
		IComponent* AddComponent(COMPONENT_TYPE type);
		IComponent* GetComponent(COMPONENT_TYPE type);
		
		GAMEOBJECT_TYPE GetType() const noexcept;
		void SetName(std::string name) noexcept;
		const std::string& GetName() const noexcept;
		void SetOwner(void* owner) noexcept;
		void* GetOwner() const noexcept;

		/* Is this object active for collision, drawing, etc. */
		bool IsActive() const noexcept;
		/* Set this object active for collision, drawing, etc. */
		void SetActive(bool v) noexcept;
		/* Is this object to be GC */
		bool IsGC() const noexcept;
		/* Set this object to be GC, calling this function also calls SetActive(flase) */
		void SetGCToTrue() noexcept;

	protected:
		bool m_active;
		bool m_gc;
		GAMEOBJECT_TYPE m_type;
		void* m_owner;
		EventManager m_eventManager;
		ComponentManager m_componentManager;
		std::string m_name;
		/* Child array */
		std::vector<IGameObject*> m_children;
		/* Call back maps */
		std::map<std::string, std::function<void(void)>> m_callBacks;
		
	};

	class IGameObjectCreator
	{
	public:
		/* owner = g_pRootObject.get(), pool = g_pMemoryManager.get() */
		virtual IGameObject* Create(void* owner, MemoryManager* pool, std::string name) = 0;
	};

	// Using a root object to update all IGameObject (deprecated)
	class RootObject : public IGameObject
	{
	public:
		RootObject();
		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Draw() override;
		virtual int Finalize() override;
		virtual void Serialize(std::wostream& wostream) override;
	};

	// TODO : Initialize in your application
	extern std::shared_ptr<RootObject> g_pRootObject;

}