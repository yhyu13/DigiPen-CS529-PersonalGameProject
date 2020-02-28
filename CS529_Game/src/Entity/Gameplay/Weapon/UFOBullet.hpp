#pragma once
#include "Common/Event/Event.hpp"
#include "src/Entity/Gameplay/Interface/IWeapon.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{
	namespace Entity
	{
		class UFOBullet : public IWeapon
		{
		public:
			UFOBullet() = delete;
			explicit UFOBullet(void* owner, std::string name) noexcept : IWeapon(WEAPON_TYPE::UFO_BULLET, owner, name)
			{
				m_eventManager.AddEventRegistry(EventType::ON_COMPONENTHIT);
			}
			virtual int Tick(double deltaTime) override;
			virtual void HandleEvent(std::weak_ptr<Event> e) override
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
			/*
				Hitting UFO:
					Shrink the UFOBullet to the colliding object
			*/
			void OnComponentHit(std::weak_ptr<Event> e);
		};
	}

	class UFOBulletObjectCreator : public IWeaponCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IWeapon* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "UFOBullet0") override
		{
			if (pool)
			{
				return pool->New<Entity::UFOBullet>(owner, name);
			}
			else
			{
				return new Entity::UFOBullet(owner, name);
			}
		}
	};
}