#pragma once
#include "Common/Event/Event.hpp"
#include "src/Entity/Gameplay/Interface/IWeapon.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{
	namespace Entity
	{
		class KineticGunExplosion : public IWeapon
		{
		public:
			KineticGunExplosion() = delete;
			explicit KineticGunExplosion(void* owner, std::string name) noexcept
				:
				IWeapon(WEAPON_TYPE::WEAPON_MISSILE_EXPLOSION, owner, name),
				life_time_timer(0.0)
			{
			}
			virtual int Tick(double deltaTime) override;

		private:
			double life_time_timer;
		};
	}

	class KineticGunExplosionObjectCreator : public IWeaponCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IWeapon* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "KineticGunExplosion0") override
		{
			if (pool)
			{
				return pool->New<Entity::KineticGunExplosion>(owner, name);
			}
			else
			{
				return new Entity::KineticGunExplosion(owner, name);
			}
		}
	};
}