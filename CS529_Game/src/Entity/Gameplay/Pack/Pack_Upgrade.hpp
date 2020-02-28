#pragma once
#include "src/Entity/Gameplay/Interface/IMyGameObject.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{
	namespace Entity
	{
		class Pack_Upgrade : public IMyGameObject
		{
		public:
			Pack_Upgrade() = delete;
			explicit Pack_Upgrade(void* owner, std::string name) noexcept 
				: 
				IMyGameObject(MYGAMEOBJECT_TYPE::Item_UPGRADE, owner, name),
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

	class Pack_UpgradeObjectCreator : public IMyGameObjectCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IMyGameObject* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "Pack_Upgrade0") override
		{
			if (pool)
			{
				return pool->New<Entity::Pack_Upgrade>(owner, name);
			}
			else
			{
				return new Entity::Pack_Upgrade(owner, name);
			}
		}
	};
}