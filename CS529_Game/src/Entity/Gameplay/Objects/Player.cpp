#include "Player.hpp"
#include "Common/Utility/Utility.hpp"
#include "Common/BaseApplication.hpp"
#include "Common/Event/EventDispatcher.hpp"
#include "src/Component/Graphics/Sprite.hpp"
#include "src/Component/Graphics/SceneComponent.hpp"
#include "src/Component/Gameplay/HealthPoint.hpp"
#include "src/Component/Gameplay/WeaponarySystem.hpp"
#include "src/SceneManager.hpp"

int My::Entity::Player::Tick(double deltaTime)
{
	auto hp_com = static_cast<Component::HealthPoint*>(GetComponent(COMPONENT_TYPE::HEALTHPOINT));
	if (hp_com->IsDeplited())
	{
		// Deactivate object (the gameObjectResourceManager will do gc)
		SetGCToTrue();
		m_callBacks["Defeat"]();
	}
	IMyGameObject::Tick(deltaTime);
	return 0;
}

int My::Entity::Player::Draw()
{
	if (auto sceneCom = m_componentManager.GetComponent(COMPONENT_TYPE::SCENECOMPONENT))
	{
		static_cast<Component::SceneComponent*>(sceneCom)->Draw();
	}
	return 0;
}

void My::Entity::Player::OnComponentHit(std::weak_ptr<Event> e)
{
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::DeathEvent*>(_e.get());
	auto other = reinterpret_cast<IMyGameObject*>(event->other);
	if (other->GetType() == MYGAMEOBJECT_TYPE::UFO)
	{
		DEBUG_PRINT("Player collides with ");
		DEBUG_PRINT(other->GetName() << "\n");
		{
			// damange to self
			auto hp_com = static_cast<Component::HealthPoint*>(GetComponent(COMPONENT_TYPE::HEALTHPOINT));
			if (!hp_com->IsLockHP())
			{
				static_cast<Component::Sprite*>(GetComponent(COMPONENT_TYPE::SPRITE))->StartHitFlash();
				std::lock_guard<std::mutex> lock1(hp_com->m_mutex);
				hp_com->Reduce(COLLISION_HP_REDUCE);
			}
		}
	}
	else if (other->GetType() == MYGAMEOBJECT_TYPE::Item_HP)
	{
		auto hp_com = static_cast<Component::HealthPoint*>(GetComponent(COMPONENT_TYPE::HEALTHPOINT));
		std::lock_guard<std::mutex> lock1(hp_com->m_mutex);
		hp_com->Restore(PLAYER_HP_RESTORE);
	}
	else if (other->GetType() == MYGAMEOBJECT_TYPE::Item_SHIELD)
	{
		g_pEventDispatcher->DispatchToGameObject(-1.0, std::make_shared<Events::LockHPEvent>(), this);
		g_pEventDispatcher->DispatchToGameObject(PLAYER_LOCKHP_PERIOD, std::make_shared<Events::UnLockHPEvent>(), this);
	}
	else if (other->GetType() == MYGAMEOBJECT_TYPE::Item_UPGRADE)
	{
		auto p_weapon_com = static_cast<Component::WeaponarySystem*>(GetComponent(COMPONENT_TYPE::WEAPONARYSYSTEM));
		p_weapon_com->UpgradeWeapons();
	}
}

void My::Entity::Player::OnEnemyDeath(std::weak_ptr<Event> e)
{
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::ComponentHitEvent*>(_e.get());
	auto other = reinterpret_cast<IMyGameObject*>(event->other);
	if (other->GetType() == MYGAMEOBJECT_TYPE::UFO)
	{
		wPRINT(str2wstr(other->GetName() + " has been destroied\n"));
		// TODO : Change victory condition
		if (++head_count == WINNING_HEAD_COUNT)
		{
			m_callBacks["Victory"]();
		}
		LoadWaveByHeadCount(head_count);
	}
}

void My::Entity::Player::OnLockHP(std::weak_ptr<Event> e)
{
	static_cast<Component::Sprite*>(GetComponent(COMPONENT_TYPE::SPRITE))->SetCurrentSpriteByName("Shield");
	static_cast<Component::HealthPoint*>(GetComponent(COMPONENT_TYPE::HEALTHPOINT))->LockHP();
}

void My::Entity::Player::OnUnLockHP(std::weak_ptr<Event> e)
{
	static_cast<Component::Sprite*>(GetComponent(COMPONENT_TYPE::SPRITE))->SetCurrentSpriteByName("Default");
	static_cast<Component::HealthPoint*>(GetComponent(COMPONENT_TYPE::HEALTHPOINT))->UnLockHP();
}

// TODO : Change load wave conditions
void My::LoadWaveByHeadCount(int head_count)
{
	switch (head_count)
	{
	case 0:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave1";
		break;
	case 2:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave2";
		break;
	case 5:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave3";
		break;
	case 8:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave4";
		break;
	case 11:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave5";
		break;
	case 14:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave6";
		break;
	case 25:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave7";
		break;
	case 26:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave8";
		break;
	case 31:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave9";
		break;
	case 33:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave10";
		break;
	case 35:
		g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave11";
		break;
	default:
		break;
	}
}
