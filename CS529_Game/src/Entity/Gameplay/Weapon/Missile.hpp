#pragma once
#include "Common/Event/Event.hpp"
#include "src/Entity/Gameplay/Interface/IWeapon.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{
	namespace Entity
	{
		class Missle : public IWeapon
		{
		public:
			Missle() = delete;
			explicit Missle(void* owner, std::string name) noexcept
				:
				IWeapon(WEAPON_TYPE::WEAPON_MISSILE, owner, name),
				bullet_id(0),
				fire_period(WEAPON_MISSILE_FIRE_PERIOD),
				fire_period_timer(WEAPON_MISSILE_FIRE_PERIOD)
			{
			}
			virtual int Tick(double deltaTime) override;
			virtual int Draw() override;

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

	class MissleObjectCreator : public IWeaponCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IWeapon* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "Missle0") override
		{
			if (pool)
			{
				return pool->New<Entity::Missle>(owner, name);
			}
			else
			{
				return new Entity::Missle(owner, name);
			}
		}
	};
}