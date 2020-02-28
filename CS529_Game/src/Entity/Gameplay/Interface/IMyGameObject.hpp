#pragma once

#include "MyObjectType.hpp"
#include "Common/Entity/IGameObject.hpp"
#include "Common/Component/ComponentManager.hpp"
#include "Common/MemoryManager.hpp"
#include "Common/Event/EventManager.hpp"

#include "src/Component/Graphics/Sprite.hpp"
#include "src/Component/Graphics/SceneComponent.hpp"

namespace My
{
	/**
	Comment :
	To create a custom weapon
	1. Add a new element to the enum type in the weapon type header
	2. Create a derived class which inherits from IMyGameObject
	3. Create a derived class which inherits from IMyGameObjectCreator
	4. RegisterSerialization (g_weaponSerializationMap) in your initializer
	5. RegisterCreator (g_weaponCreatorMap) in your initializer
	*/
	class IMyGameObject : public IGameObject
	{
	public:
		IMyGameObject(const IMyGameObject&) = delete;
		IMyGameObject& operator=(const IMyGameObject&) = delete;
		explicit IMyGameObject(MYGAMEOBJECT_TYPE type, void* owner, std::string name) noexcept;
		MYGAMEOBJECT_TYPE GetType() const noexcept;
	protected:
		MYGAMEOBJECT_TYPE m_type;
	};

	class IMyGameObjectCreator
	{
	public:
		/* owner = g_pRootObject.get(), pool = g_pMemoryManager.get() */
		virtual IMyGameObject* Create(void* owner, MemoryManager* pool, std::string name) = 0;
	};

	// TODO : Initialize in your application
	extern std::map<std::string, MYGAMEOBJECT_TYPE> g_myGameObjectSerializationMap;
	extern std::map<MYGAMEOBJECT_TYPE, std::shared_ptr<IMyGameObjectCreator>> g_myGameObjectCreatorMap;
}