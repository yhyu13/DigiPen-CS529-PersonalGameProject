#pragma once
#include "Common/Event/Event.hpp"
#include "src/Entity/Gameplay/Interface/IWeapon.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{
	namespace Entity
	{
		class MissleBullet : public IWeapon
		{
		public:
			MissleBullet() = delete;
			explicit MissleBullet(void* owner, std::string name) noexcept : IWeapon(WEAPON_TYPE::WEAPON_MISSILE_BULLET, owner, name)
			{
				m_eventManager.AddEventRegistry(EventType::ON_COMPONENTHIT);
			}
			virtual int Tick(double deltaTime) override;
			virtual int Draw() override;
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
					Shrink the MissleBullet to the colliding object
			*/
			void OnComponentHit(std::weak_ptr<Event> e);
		};
	}

	class MissleBulletObjectCreator : public IWeaponCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IWeapon* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "MissleBullet0") override
		{
			if (pool)
			{
				return pool->New<Entity::MissleBullet>(owner, name);
			}
			else
			{
				return new Entity::MissleBullet(owner, name);
			}
		}
	};
}