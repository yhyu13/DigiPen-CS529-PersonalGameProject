#pragma once
#include "src/Entity/Gameplay/Interface/IMyGameObject.hpp"

namespace My
{
	namespace Entity
	{
		class Background : public IMyGameObject
		{
		public:
			Background() = delete;
			explicit Background(void* owner, std::string name) noexcept
				:IMyGameObject(MYGAMEOBJECT_TYPE::BACKGROUND, owner, name)
			{
			}
			virtual int Draw() override;
		};
	}

	class BackgroundObjectCreator : public IMyGameObjectCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IMyGameObject* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "Background0") override
		{
			if (pool)
			{
				return pool->New<Entity::Background>(owner, name);
			}
			else
			{
				return new Entity::Background(owner, name);
			}
		}
	};
}