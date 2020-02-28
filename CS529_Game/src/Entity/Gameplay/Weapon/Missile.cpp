#include "Missile.hpp"
#include "Common/Utility/Utility.hpp"
#include "Common/Entity/GameObjectResourceManager.hpp"
#include "src/Component/Graphics/Sprite.hpp"
#include "Common/Component/Physics/Body.hpp"
#include "Common/Component/ComponentManager.hpp"

int My::Entity::Missle::Tick(double deltaTime)
{
	if (deltaTime != 0.0 && IsActive())
	{
		if ((fire_period_timer += deltaTime) >= fire_period)
		{
			fire_period_timer = 0.0;
			switch (m_level)
			{
			case 1:
				SpawnBullet1();
				break;
			case 2:
				SpawnBullet2();
				break;
			case 3:
				SpawnBullet3();
				break;
			default:
				break;
			}
		}
	}
	IWeapon::Tick(deltaTime);
	return 0;
}

int My::Entity::Missle::Draw()
{
	return 0;
}

My::IWeapon* My::Entity::Missle::SpawnBullet()
{
	char weaponType[256] = "MissileBullet";
	std::string weaponName("MB_" + Str(bullet_id++));
	// Serialize the game object and set its owner to be the root object.
	if (g_weaponSerializationMap.find(weaponType) != g_weaponSerializationMap.end())
	{
		auto pNewObject = LoadIWeaponFromFile(this, weaponType, weaponName);
		// Add game object under the resource manager.
		if (1 == g_pGameObjectResourceManager->AddResource(pNewObject, weaponName))
		{
			std::string msg("Adding resource : '" + std::string(weaponName) + "' has failed.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
		// Set bullet to the current weapon level
		pNewObject->SetUpgradeLevel(m_level);
		return pNewObject;
	}
	else
	{
		std::string msg("File does not provide a valid game object of type: " + std::string(weaponName));
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
	}
}

void My::Entity::Missle::SpawnBullet1()
{
	Vector2D p;
	auto owner = static_cast<ComponentManager*>(static_cast<IComponent*>(GetOwner())->GetOwner());
	auto p_owner_transformCom = static_cast<Component::Body*>(owner->GetComponent(COMPONENT_TYPE::BODY));
	auto p_owner_spriteCom = static_cast<Component::Sprite*>(owner->GetComponent(COMPONENT_TYPE::SPRITE));
	{
		std::lock_guard<std::mutex> lock1(p_owner_transformCom->m_mutex);
		Vector2DSet(&p, &p_owner_transformCom->m_position);
	}
	
	for (int i = -1; i <= 1; i+=2)
	{
		Vector2D a(i*-50.0f, -WEAPON_MISSILE_BULLET_ACC);
		Vector2D v(i*100.0f, 0.0f);
		auto pNewObject = SpawnBullet();
		// Set bullet position and velocity
		auto p_transformCom = static_cast<Component::Body*>(pNewObject->GetComponent(COMPONENT_TYPE::BODY));
		auto p_spriteCom = static_cast<Component::Sprite*>(pNewObject->GetComponent(COMPONENT_TYPE::SPRITE));
		{
			std::lock_guard<std::mutex> lock2(p_transformCom->m_mutex);
			Vector2DSet(&p_transformCom->m_position, &p);
			p_transformCom->m_position.y += (p_owner_spriteCom->m_rcSprite.h / 2);
			Vector2DSet(&p_transformCom->m_velocity, &v);
			Vector2DSet(&p_transformCom->m_acceleration, &a);
		}
	}
}

void My::Entity::Missle::SpawnBullet2()
{
	SpawnBullet1();
}

void My::Entity::Missle::SpawnBullet3()
{
	SpawnBullet1();
}
