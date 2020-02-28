#include "KineticGunBullet.hpp"
#include "Common/Utility/Utility.hpp"
#include "Common/Component/ComponentManager.hpp"
#include "Common/Component/Physics/Body.hpp"
#include "Common/Entity/GameObjectResourceManager.hpp"
#include "src/Component/Gameplay/HealthPoint.hpp"
#include "src/Platform/Win32_SDL/WinSDLOpenGLApplication.hpp"

int My::Entity::KineticGunBullet::Tick(double deltaTime)
{
	if (deltaTime != 0.0)
	{
		auto temp = GetComponent(COMPONENT_TYPE::BODY);
		auto p_App = static_cast<WinSDLOpenGLApplication*>(g_pBaseApplication.get());
		if (temp && p_App)
		{
			int SCREEN_WIDTH, SCREEN_HEIGHT;
			SDL_GetWindowSize(p_App->GetWindow(), &SCREEN_WIDTH, &SCREEN_HEIGHT);

			auto p_transformCom = static_cast<Component::Body*>(temp);
			{
				std::lock_guard<std::mutex> lock(p_transformCom->m_mutex);
				/* collide with edges of screen */
				if (p_transformCom->m_position.x < 0 || p_transformCom->m_position.x > SCREEN_WIDTH || p_transformCom->m_position.y < 0 || p_transformCom->m_position.y > SCREEN_HEIGHT) {
					SetGCToTrue();
				}
			}
		}
	}
	IWeapon::Tick(deltaTime);
	return 0;
}

void My::Entity::KineticGunBullet::OnComponentHit(std::weak_ptr<Event> e)
{
	static int count = 0;
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::DeathEvent*>(_e.get());
	auto other = reinterpret_cast<IMyGameObject*>(event->other);

	if (other->GetType() == MYGAMEOBJECT_TYPE::UFO)
	{
		DEBUG_wPRINT(str2wstr(m_name + " collides with "));
		DEBUG_wPRINT(str2wstr(other->GetName()) << "\n");
		{
			auto hp_com = static_cast<Component::HealthPoint*>(other->GetComponent(COMPONENT_TYPE::HEALTHPOINT));
			std::lock_guard<std::mutex> lock1(hp_com->m_mutex);
			hp_com->Reduce(WEAPON_KINETICGUN_DAM_BASE);
		}
		char weaponType[256] = "KineticGunExplosion";
		std::string weaponName(m_name + "_explosion" + Str(++count));
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
			auto p_other_sprite = static_cast<Component::Sprite*>(pNewObject->GetComponent(COMPONENT_TYPE::SPRITE));
			auto p_ohter_transformCom = static_cast<Component::Body*>(pNewObject->GetComponent(COMPONENT_TYPE::BODY));
			auto p_transformCom = static_cast<Component::Body*>(GetComponent(COMPONENT_TYPE::BODY));
			{
				std::lock_guard<std::mutex> lock1(p_ohter_transformCom->m_mutex);
				std::lock_guard<std::mutex> lock2(p_transformCom->m_mutex);
				Vector2DSet(&p_ohter_transformCom->m_position, &p_transformCom->m_position);
				p_ohter_transformCom->m_position.y -= static_cast<float>(p_other_sprite->m_rcSprite.h / 2);
			}
		}
		else
		{
			std::string msg("File does not provide a valid game object of type: " + std::string(weaponName));
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
		SetGCToTrue();
	}
}
