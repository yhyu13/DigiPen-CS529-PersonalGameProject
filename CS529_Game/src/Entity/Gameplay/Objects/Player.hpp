#pragma once

#include "src/Entity/Gameplay/Interface/IMyGameObject.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{

	void LoadWaveByHeadCount(int head_count);

	namespace Entity
	{
		class Player : public IMyGameObject
		{
		public:
			Player() = delete;
			explicit Player(void* owner, std::string name) noexcept 
				: 
				IMyGameObject(MYGAMEOBJECT_TYPE::PLAYER, owner, name),
				head_count(0)
			{
				m_eventManager.AddEventRegistry(EventType::ON_COMPONENTHIT);
				m_eventManager.AddEventRegistry(EventType::ON_DEATH);
				m_eventManager.AddEventRegistry(EventType::ON_LOCKHP);
				m_eventManager.AddEventRegistry(EventType::ON_UNLOCKHP);
			}
			virtual int Tick(double deltaTime) override;
			virtual int Draw() override;
			virtual void HandleEvent(std::weak_ptr<Event> e) override
			{
				std::shared_ptr<Event> _e = e.lock();
				switch (_e->GetType())
				{
				case EventType::ON_DEATH:
					OnEnemyDeath(_e);
					break;
				case EventType::ON_COMPONENTHIT:
					OnComponentHit(_e);
					break;
				case EventType::ON_LOCKHP:
					OnLockHP(_e);
					break;
				case EventType::ON_UNLOCKHP:
					OnUnLockHP(_e);
					break;
				default:
					break;
				}
			}
			// Custom events
			void OnComponentHit(std::weak_ptr<Event> e);
			void OnEnemyDeath(std::weak_ptr<Event> e);
			void OnLockHP(std::weak_ptr<Event> e);
			void OnUnLockHP(std::weak_ptr<Event> e);
		public:
			int head_count;
		};
	}

	class PlayerObjectCreator : public IMyGameObjectCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IMyGameObject* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "Player0") override
		{
			if (pool)
			{
				return pool->New<Entity::Player>(owner, name);
			}
			else
			{
				return new Entity::Player(owner, name);
			}
		}
	};
}