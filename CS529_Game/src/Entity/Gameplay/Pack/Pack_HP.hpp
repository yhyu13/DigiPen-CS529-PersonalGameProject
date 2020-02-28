#pragma once
#include "src/Entity/Gameplay/Interface/IMyGameObject.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{
	namespace Entity
	{
		class Pack_HP : public IMyGameObject
		{
		public:
			Pack_HP() = delete;
			explicit Pack_HP(void* owner, std::string name) noexcept 
				: 
				IMyGameObject(MYGAMEOBJECT_TYPE::Item_HP, owner, name),
				life_time(PACK_LIFETIME)
			{
				m_eventManager.AddEventRegistry(EventType::ON_COMPONENTHIT);
			}
			virtual int Tick(double deltaTime) override;
			virtual int Draw() override;
			virtual void HandleEvent(std::weak_ptr<Event> e)
			{
				if (IsActive())
				{
					std::shared_ptr<Event> _e = e.lock();
					switch (_e->GetType())
					{
					case EventType::ON_COMPONENTHIT:
						OnComponentHit(_e);
						break;

					default:
						break;
					}
				}
			}
			void OnComponentHit(std::weak_ptr<Event> e);
		private:
			double life_time;
		};
	}

	class Pack_HPObjectCreator : public IMyGameObjectCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IMyGameObject* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "Pack_HP0") override
		{
			if (pool)
			{
				return pool->New<Entity::Pack_HP>(owner, name);
			}
			else
			{
				return new Entity::Pack_HP(owner, name);
			}
		}
	};
}