#pragma once
#include "Common/Event/Event.hpp"
#include "src/Entity/Gameplay/Interface/IWeapon.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{
	namespace Entity
	{
		class Lazer : public IWeapon
		{
		public:
			Lazer() = delete;
			explicit Lazer(void* owner, std::string name) noexcept 
				: 
				IWeapon(WEAPON_TYPE::WEAPON_LAZER, owner, name),
				bullet_id(0),
				fire_period(WEAPON_LAZER_FIRE_PERIOD),
				fire_period_timer(WEAPON_LAZER_FIRE_PERIOD)
			{
				m_eventManager.AddEventRegistry(EventType::ON_COMPONENTHIT);
			}
			virtual int Tick(double deltaTime) override;
			virtual void HandleEvent(std::weak_ptr<Event> e) override
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
			/*
				Hitting UFO:
					Shrink the lazer to the colliding object
			*/
			void OnComponentHit(std::weak_ptr<Event> e);
			void ResetSize();

			IWeapon* SpawnBullet();
			void SpawnBullet1();
			void SpawnBullet2();
			void SpawnBullet3();

		private:
			uint64_t bullet_id;
			double fire_period;
			double fire_period_timer;
		};
	}

	class LazerObjectCreator : public IWeaponCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IWeapon* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "Lazer0") override
		{
			if (pool)
			{
				return pool->New<Entity::Lazer>(owner, name);
			}
			else
			{
				return new Entity::Lazer(owner, name);
			}
		}
	};
}