#include "MissileExplosion.hpp"
#include "Common/Component/Physics/Body.hpp"

int My::Entity::MissileExplosion::Tick(double deltaTime)
{
	if (deltaTime != 0.0)
	{
		if ((life_time_timer += deltaTime) >= WEAPON_MISSILE_EXPLOSION_LIFETIME)
		{
			SetGCToTrue();
		}
	}

	IWeapon::Tick(deltaTime);
	return 0;
}
