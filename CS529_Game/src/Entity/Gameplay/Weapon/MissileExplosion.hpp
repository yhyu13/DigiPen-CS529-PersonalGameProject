#pragma once
#include "Common/Event/Event.hpp"
#include "src/Entity/Gameplay/Interface/IWeapon.hpp"
#include "src/Event/MyEvent.hpp"

namespace My
{
	namespace Entity
	{
		class MissileExplosion : public IWeapon
		{
		public:
			MissileExplosion() = delete;
			explicit MissileExplosion(void* owner, std::string name) noexcept
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

	class MissileExplosionObjectCreator : public IWeaponCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IWeapon* Create(void* owner = nullptr, MemoryManager* pool = nullptr, std::string name = "MissileExplosion0") override
		{
			if (pool)
			{
				return pool->New<Entity::MissileExplosion>(owner, name);
			}
			else
			{
				return new Entity::MissileExplosion(owner, name);
			}
		}
	};
}