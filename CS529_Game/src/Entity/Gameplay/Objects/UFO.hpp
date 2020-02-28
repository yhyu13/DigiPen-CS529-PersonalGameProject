#pragma once
#include "src/Entity/Gameplay/Interface/IMyGameObject.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{
	namespace Entity
	{
		class UFO : public IMyGameObject
		{
		public:
			UFO() = delete;
			explicit UFO(void* owner, std::string name) noexcept : IMyGameObject(MYGAMEOBJECT_TYPE::UFO, owner, name)
			{
				m_eventManager.AddEventRegistry(EventType::ON_COMPONENTHIT);
				m_eventManager.AddEventRegistry(EventType::ON_TELEPORT);

				// set random seed at current time
				std::map<double, int> cumulative;
				typedef std::map<double, char>::iterator It;
				cumulative[PACK_HP_THRESHOLD] = PACK_HP_INDEX;
				cumulative[PACK_UPGRADE_THRESHOLD] = PACK_UPGRADE_INDEX;
				cumulative[PACK_SHIELD_THRESHOLD] = PACK_SHIELD_INDEX;
				cumulative[1.00] = 0;
				const int numTests = 2;
				for (int i = 0; i != numTests; ++i)
				{
					double linear = rand() * 1.0 / RAND_MAX;
					m_drop_list.push_back(cumulative.upper_bound(linear)->second);
				}
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
					case EventType::ON_TELEPORT:
						OnSwitchFireMode(_e);
						break;
					default:
						break;
					}
				}
			}
			void OnComponentHit(std::weak_ptr<Event> e);
			void OnSwitchFireMode(std::weak_ptr<Event> e);
			void DropPack();
		private:
			std::vector<int> m_drop_list;
		};
	}

	class UFOObjectCreator : public IMyGameObjectCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IMyGameObject* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "UFO0") override
		{
			if (pool)
			{
				return pool->New<Entity::UFO>(owner, name);
			}
			else
			{
				return new Entity::UFO(owner, name);
			}
		}
	};
}