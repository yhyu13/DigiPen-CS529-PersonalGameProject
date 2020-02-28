#include "Lazer.hpp"
#include "Common/Entity/IGameObject.hpp"
#include "Common/Utility/Utility.hpp"
#include "Common/Component/Physics/Body.hpp"
#include "Common/Component/ComponentManager.hpp"
#include "Common/BaseApplication.hpp"
#include "Common/Entity/GameObjectResourceManager.hpp"
#include "src/Component/Graphics/Sprite.hpp"
#include "src/Component/Graphics/SceneComponent.hpp"
#include "src/Component/Gameplay/HealthPoint.hpp"

int My::Entity::Lazer::Tick(double deltaTime)
{
	if (deltaTime != 0.0 && IsActive())
	{
		ResetSize();
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

void My::Entity::Lazer::OnComponentHit(std::weak_ptr<Event> e)
{
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::DeathEvent*>(_e.get());
	auto other = reinterpret_cast<IMyGameObject*>(event->other);


	if (other->GetType() == MYGAMEOBJECT_TYPE::UFO)
	{
		DEBUG_wPRINT(str2wstr(m_name + " collides with "));
		DEBUG_wPRINT(str2wstr(other->GetName()) << "\n");
		{
			auto hp_com = static_cast<Component::HealthPoint*>(other->GetComponent(COMPONENT_TYPE::HEALTHPOINT));
			std::lock_guard<std::mutex> lock3(hp_com->m_mutex);
			float damage = 0.0f;
			switch (m_level)
			{
			case 1:
				damage = WEAPON_LAZER_DAM_BASE;
				break;
			case 2:
				damage = WEAPON_LAZER_DAM_BASE + 1.0f;
				break;
			case 3:
				damage = WEAPON_LAZER_DAM_BASE + 2.0f;
				break;
			default:
				break;
			}
			hp_com->Reduce(ADJUST_fVALUE_60HZ(damage, BaseApplication::frametime));
		}
	}
}

void My::Entity::Lazer::ResetSize()
{
	if (!BaseApplication::IsPuased() && IsActive())
	{
		auto owner = static_cast<ComponentManager*>(static_cast<IComponent*>(GetOwner())->GetOwner());
		auto p_owner_transformCom = static_cast<Component::Body*>(owner->GetComponent(COMPONENT_TYPE::BODY));
		auto p_owner_spriteCom = static_cast<Component::Sprite*>(owner->GetComponent(COMPONENT_TYPE::SPRITE));
		auto p_transformCom = static_cast<Component::Body*>(GetComponent(COMPONENT_TYPE::BODY));
		auto p_spriteCom = static_cast<Component::Sprite*>(GetComponent(COMPONENT_TYPE::SPRITE));
		{
			std::lock_guard<std::mutex> lock1(p_owner_transformCom->m_mutex);
			std::lock_guard<std::mutex> lock2(p_transformCom->m_mutex);
			Vector2DSet(&p_transformCom->m_position, &p_owner_transformCom->m_position);
			p_spriteCom->m_rcSprite.h = WEAPON_LAZER_ORIGIN_HIEGHT;
			p_transformCom->m_position.y -= (p_spriteCom->m_rcSprite.h / 2 + p_owner_spriteCom->m_rcSprite.h / 2);
			static_cast<Collision::Rectangle*>(p_transformCom->m_shape)->m_height = static_cast<float>(p_spriteCom->m_rcSprite.h);
		}
	}
}

IWeapon* My::Entity::Lazer::SpawnBullet()
{
	char weaponType[256] = "LazerBeam";
	std::string weaponName("003_LB_" + Str(bullet_id++));
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

void My::Entity::Lazer::SpawnBullet1()
{
	Vector2D p;
	auto owner = static_cast<ComponentManager*>(static_cast<IComponent*>(GetOwner())->GetOwner());
	auto p_owner_transformCom = static_cast<Component::Body*>(owner->GetComponent(COMPONENT_TYPE::BODY));
	auto p_owner_spriteCom = static_cast<Component::Sprite*>(owner->GetComponent(COMPONENT_TYPE::SPRITE));
	{
		std::lock_guard<std::mutex> lock1(p_owner_transformCom->m_mutex);
		Vector2DSet(&p, &p_owner_transformCom->m_position);
	}
	Vector2D v(0.0f, -WEAPON_LAZER_BULLET_SPEED);

	for (int i = -1; i <= 1; i+=2)
	{
		auto pNewObject = SpawnBullet();
		// Set bullet position and velocity
		auto p_transformCom = static_cast<Component::Body*>(pNewObject->GetComponent(COMPONENT_TYPE::BODY));
		auto p_spriteCom = static_cast<Component::Sprite*>(pNewObject->GetComponent(COMPONENT_TYPE::SPRITE));
		{
			std::lock_guard<std::mutex> lock2(p_transformCom->m_mutex);
			Vector2DSet(&p_transformCom->m_position, &p);
			p_transformCom->m_position.y += (p_owner_spriteCom->m_rcSprite.h / 2);
			p_transformCom->m_position.x += i*(p_owner_spriteCom->m_rcSprite.w *2 / 3);
			Vector2DSet(&p_transformCom->m_velocity, &v);
		}
	}
}

void My::Entity::Lazer::SpawnBullet2()
{
	SpawnBullet1();
}

void My::Entity::Lazer::SpawnBullet3()
{
	SpawnBullet1();
}
