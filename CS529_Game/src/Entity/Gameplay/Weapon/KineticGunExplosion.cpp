#include "KineticGunExplosion.hpp"
#include "Common/Component/Physics/Body.hpp"

int My::Entity::KineticGunExplosion::Tick(double deltaTime)
{
	if (deltaTime != 0.0)
	{
		if ((life_time_timer += deltaTime) >= WEAPON_KINETICGUN_EXPLOSION_LIFETIME)
		{
			SetGCToTrue();
		}
	}

	IWeapon::Tick(deltaTime);
	return 0;
}
