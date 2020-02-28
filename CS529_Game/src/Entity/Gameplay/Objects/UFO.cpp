#include "UFO.hpp"

#include "Common/Event/EventDispatcher.hpp"
#include "Common/Component/Physics/Body.hpp"
#include "src/Component/Graphics/Sprite.hpp"
#include "src/Component/Graphics/SceneComponent.hpp"
#include "src/Component/Gameplay/HealthPoint.hpp"
#include "src/Component/Logic/AIController.hpp"
#include "src/Entity/Gameplay/Interface/IWeapon.hpp"
#include "src/Platform/Win32_SDL/WinSDLAudioManager.hpp"
#include "src/Entity/Gameplay/Pack/Pack_HP.hpp"
#include "src/Entity/Gameplay/Pack/Pack_Shield.hpp"
#include "src/Entity/Gameplay/Pack/Pack_Upgrade.hpp"
#include "src/Entity/Gameplay/Pack/Pack_Score.hpp"
#include "src/SerializedLoader.hpp"

int My::Entity::UFO::Tick(double deltaTime)
{
	auto hp_com = static_cast<Component::HealthPoint*>(GetComponent(COMPONENT_TYPE::HEALTHPOINT));
	if (hp_com->IsDeplited())
	{
		// Send death event
		auto death = std::make_shared<Events::DeathEvent>(this);
		if (auto player = g_pGameObjectResourceManager->GetResourceByName("002_Player"))
		{
			death->AddSubscriber(player);
			g_pEventDispatcher->DispatchToSubscribers(-1.0, death);
		}
		// Deactivate object (the gameObjectResourceManager will do gc)
		SetGCToTrue();
		// Play destory sound effect
		g_pAudioManager->PlayChunck("Blast");
		DropPack();
	}
	IMyGameObject::Tick(deltaTime);
	return 0;
}

int My::Entity::UFO::Draw()
{
	auto temp = GetComponent(COMPONENT_TYPE::SCENECOMPONENT);
	if (temp)
	{
		static_cast<Component::SceneComponent*>(temp)->Draw();
		return 0;
	}
	return 1;
}

void My::Entity::UFO::OnComponentHit(std::weak_ptr<Event> e)
{
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::ComponentHitEvent*>(_e.get());
	auto other = static_cast<IMyGameObject*>(event->other);
	if (other->GetType() != MYGAMEOBJECT_TYPE::UFO)
	{
		static_cast<Component::Sprite*>(GetComponent(COMPONENT_TYPE::SPRITE))->StartHitFlash();
		// damage to UFO
		{
			auto hp_com = static_cast<Component::HealthPoint*>(GetComponent(COMPONENT_TYPE::HEALTHPOINT));
			std::lock_guard<std::mutex> lock1(hp_com->m_mutex);
			hp_com->Reduce(COLLISION_HP_REDUCE);
		}
	}
}

void My::Entity::UFO::OnSwitchFireMode(std::weak_ptr<Event> e)
{
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::AISwitchFireModeEvent*>(_e.get());
	static_cast<Component::AIController*>(GetComponent(COMPONENT_TYPE::AICONTROLLER))->SetFireMode(event->mode);
	static_cast<Component::AIController*>(GetComponent(COMPONENT_TYPE::AICONTROLLER))->switched_event_sent = false;
	DEBUG_PRINT("UFO switches fire mode to: " + Str(event->mode) + "\n");
}

void My::Entity::UFO::DropPack()
{
	for (int i = 0; i < m_drop_list.size() ; ++i)
	{
		std::string drop_name;
		switch (m_drop_list[i])
		{
		case 0:
			continue;
			break;
		case PACK_HP_INDEX:
			drop_name = "Pack_HP";
			break;
		case PACK_UPGRADE_INDEX:
			drop_name = "Pack_Upgrade";
			break;
		case PACK_SHIELD_INDEX:
			drop_name = "Pack_Shield";
			break;
		default:
			break;
		}
		// Serialize the game object and set its owner to be the root object.
		if (g_myGameObjectSerializationMap.find(drop_name) != g_myGameObjectSerializationMap.end())
		{
			auto pNewObject = LoadIGameObjectFromFile(drop_name.c_str(), m_name + drop_name + Str(i));
			// Add game object under the resource manager.
			if (1 == g_pGameObjectResourceManager->AddResource(pNewObject, pNewObject->GetName()))
			{
				std::string msg("Adding resource : '" + std::string(drop_name) + "' has failed.");
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
			}
			auto p_new_transformCom = static_cast<Component::Body*>(pNewObject->GetComponent(COMPONENT_TYPE::BODY));
			auto p_transformCom = static_cast<Component::Body*>(GetComponent(COMPONENT_TYPE::BODY));
			{
				std::lock_guard<std::mutex> lock1(p_transformCom->m_mutex);
				std::lock_guard<std::mutex> lock2(p_new_transformCom->m_mutex);
				Vector2DSet(&p_new_transformCom->m_position, &p_transformCom->m_position);
			}
		}
		else
		{
			std::string msg("File does not provide a valid game object of type: " + std::string(drop_name));
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
	}
}
