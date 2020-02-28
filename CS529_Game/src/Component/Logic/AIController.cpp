#include <SDL.h>
#include "AIController.hpp"
#include "Common/Entity/GameObjectResourceManager.hpp"
#include "Common/Utility/Utility.hpp"
#include "Common/Component/ComponentManager.hpp"
#include "Common/Component/Physics/Body.hpp"

#include "Common/Event/EventDispatcher.hpp"

#include "src/Event/MyEvent.hpp"

My::Component::AIController::AIController(void* owner, std::string name) 
	: 
	IComponent((COMPONENT_TYPE::AICONTROLLER), owner, name),
	switched_event_sent(false),
	fire_period(0.0),
	fire_period_timer(0.0),
	bullet_id(0),
	motion_mode(0),
	motion_mode2(0),
	motion_mode3(0),
	fire_mode(0),
	fire_mode2(0),
	fire_mode3(0),
	m_phase(RAND_F(0.0f, PI)),
	m_amplitude(RAND_F(1.0f, 2.0f)),
	m_right(RAND_F(-1.0f, 1.0f))
{
	if (m_right >= -.25f && m_right <= 0.0f)
	{
		m_right = RAND_F(-1.0f, -.25f);
	}
	else if (m_right <= .25f && m_right >= 0.0f)
	{
		m_right = RAND_F(.25f, 1.0f);
	}
	m_destination.x = 0;
	m_destination.y = 0;
}

void My::Component::AIController::Update(double deltaTime)
{
	UpdateMotion(deltaTime);
	UpdateFire(deltaTime);
}

void My::Component::AIController::Serialize(std::wostream& wostream)
{
	wostream << "[Component : AIController] ";
}

void My::Component::AIController::Serialize(std::ifstream& fstream)
{
	for (std::string line; getline(fstream, line);)
	{
		if (line[0] == ']')
		{
			return;
		}

		if (2 == sscanf_s(line.c_str(), "p (%f,%f) ", &m_destination.x, &m_destination.y))
		{
			continue;
		}

		if (0 < sscanf_s(line.c_str(), "motion mode %d %d %d ", &motion_mode, &motion_mode2, &motion_mode3))
		{
			continue;
		}

		if (0 < sscanf_s(line.c_str(), "fire mode %d %d %d ", &fire_mode, &fire_mode2, &fire_mode3))
		{
			continue;
		}
	}
}

void My::Component::AIController::UpdateMotion(double deltaTime)
{
	auto owner = static_cast<ComponentManager*>(GetOwner());
	auto temp_body = owner->GetComponent(COMPONENT_TYPE::BODY);

	if (temp_body)
	{
		auto p_bodyCom = static_cast<Component::Body*>(temp_body);
		if (motion_mode == 0)
		{
			{
				std::lock_guard<std::mutex> lock(p_bodyCom->m_mutex);
				// target is active
				Vector2D forward, diff;
				Vector2D target_pos = m_destination;
				Vector2DSub(&diff, &target_pos, &p_bodyCom->m_position);
				// Switch to stationary mode
				if (Vector2DLength(&diff) <= 100.0f)
				{
					motion_mode = 1;
					return;
				}
				Vector2DNormalize(&forward, &p_bodyCom->m_velocity);
				float diff_angle = acosf(Vector2DDotProduct(&forward, &diff) / Vector2DLength(&diff));
				float possible_angle = p_bodyCom->m_angle+diff_angle;				// Pick an angle, here I choose to add diff_angle from mAngle
				Vector2DSet(&forward, cosf(possible_angle), sinf(possible_angle));
				float dot = Vector2DDotProduct(&forward, &diff) / Vector2DLength(&diff);	// Calculate the dot product value
				if (fabs(dot - 1.0f) < 0.01f)												// The picked angle, after adding to the current angle points to the target. Success.
				{
					diff_angle = (diff_angle > UFO_ROT_SPEED) ? UFO_ROT_SPEED : diff_angle; // Limit rotaional speed
				}
				else
				{
					diff_angle *= -1.0f;
					diff_angle = (diff_angle < -UFO_ROT_SPEED) ? -UFO_ROT_SPEED : diff_angle;
				}
				p_bodyCom->m_angle += diff_angle * static_cast<float>(deltaTime) * 5.0f;
				p_bodyCom->m_velocity.x = UFO_SPEED_1 * cosf(p_bodyCom->m_angle);
				p_bodyCom->m_velocity.y = UFO_SPEED_1 * sinf(p_bodyCom->m_angle);
			}
		}

		if (motion_mode == 1)
		{
			Vector2D Up(0.0f, -100.0f);
			Vector2D Right(100.0f, 0.0f);
			// Going up and down in a way of sine curve.
			m_phase += static_cast<float>(deltaTime) * 2.0f * PI * m_amplitude;
			m_phase = fmodf(m_phase, 2 * PI);
			{
				std::lock_guard<std::mutex> lock(p_bodyCom->m_mutex);
				// Reset velocity
				Vector2DZero(&p_bodyCom->m_velocity);
				Vector2DScaleAdd(&p_bodyCom->m_velocity, &Up, &p_bodyCom->m_velocity, sinf(m_phase));
				if (p_bodyCom->m_position.x <= 50 || p_bodyCom->m_position.x >= 550)
				{
					m_right = -m_right;
				}
				Vector2DScaleAdd(&p_bodyCom->m_velocity, &Right, &p_bodyCom->m_velocity, m_right);
				p_bodyCom->m_angle = 0.0f;
			}
		}
	}
}

void My::Component::AIController::UpdateFire(double deltaTime)
{
	auto owner = static_cast<ComponentManager*>(GetOwner());
	auto p_owner_transformCom = static_cast<Component::Body*>(owner->GetComponent(COMPONENT_TYPE::BODY));
	if (fire_mode == 0)
	{
		fire_period = UFO_FIRE_MODE_1_FIRE_PERIOD;
		std::string name = "UFOBullet";
		if ((fire_period_timer += deltaTime) >= fire_period)
		{
			fire_period_timer = 0.0;
			auto pNewObject = SpawnBullet(name,1);
			auto p_new_transformCom = static_cast<Component::Body*>(pNewObject->GetComponent(COMPONENT_TYPE::BODY));
			{
				Vector2D v(RAND_F(-100.0f, 100.0f), 200.0f);
				std::lock_guard<std::mutex> lock1(p_owner_transformCom->m_mutex);
				std::lock_guard<std::mutex> lock2(p_new_transformCom->m_mutex);
				Vector2DSet(&p_new_transformCom->m_position, &p_owner_transformCom->m_position);
				Vector2DSet(&p_new_transformCom->m_velocity, &v);
			}
		}
		if (fire_mode2 != 0 && !switched_event_sent)
		{
			switched_event_sent = true;
			g_pEventDispatcher->DispatchToGameObject(6.0, std::make_shared<Events::AISwitchFireModeEvent>(fire_mode2), static_cast<IGameObject*>(owner->GetOwner()));
		}
	}
	else if (fire_mode == 1)
	{
		fire_period = UFO_FIRE_MODE_2_FIRE_PERIOD;
		std::string name = "UFOBullet";
		if ((fire_period_timer += deltaTime) >= fire_period)
		{
			fire_period_timer = 0.0;
			float vx = -100.0f;
			for (int i = 0; i < 10; ++i)
			{
				auto pNewObject = SpawnBullet(name, 2);
				auto p_new_transformCom = static_cast<Component::Body*>(pNewObject->GetComponent(COMPONENT_TYPE::BODY));
				{
					Vector2D v(vx - i * vx / 5, 200.0f);
					std::lock_guard<std::mutex> lock1(p_owner_transformCom->m_mutex);
					std::lock_guard<std::mutex> lock2(p_new_transformCom->m_mutex);
					Vector2DSet(&p_new_transformCom->m_position, &p_owner_transformCom->m_position);
					Vector2DSet(&p_new_transformCom->m_velocity, &v);
				}
			}
		}
		if (fire_mode3 != 0 && !switched_event_sent)
		{
			switched_event_sent = true;
			g_pEventDispatcher->DispatchToGameObject(4.0, std::make_shared<Events::AISwitchFireModeEvent>(RAND_I(0,3)), static_cast<IGameObject*>(owner->GetOwner()));
		}
	}
	else if (fire_mode == 2)
	{
		fire_period = UFO_FIRE_MODE_3_FIRE_PERIOD;
		std::string name = "UFOBullet";
		float vx = -100.0f;
		float angle = PI / 4.0f;
		static int i = 0;
		if ((fire_period_timer += deltaTime) >= fire_period)
		{
			fire_period_timer = 0.0;
			if (++i > 10)
			{
				i = 0;
			}
			auto pNewObject = SpawnBullet(name,3);
			auto p_new_transformCom = static_cast<Component::Body*>(pNewObject->GetComponent(COMPONENT_TYPE::BODY));
			{
				Vector2D v(vx - i * vx / 5, 200.0f);
				std::lock_guard<std::mutex> lock1(p_owner_transformCom->m_mutex);
				std::lock_guard<std::mutex> lock2(p_new_transformCom->m_mutex);
				Vector2DSet(&p_new_transformCom->m_position, &p_owner_transformCom->m_position);
				Vector2DSet(&p_new_transformCom->m_velocity, &v);
				p_new_transformCom->m_angle = angle - i * angle / 5;
			}
		}
		if (!switched_event_sent)
		{
			switched_event_sent = true;
			g_pEventDispatcher->DispatchToGameObject(4.0, std::make_shared<Events::AISwitchFireModeEvent>(RAND_I(0, 3)), static_cast<IGameObject*>(owner->GetOwner()));
		}
	}
}

void My::Component::AIController::SetFireMode(int mode)
{
	fire_mode = mode;
}

IWeapon* My::Component::AIController::SpawnBullet(std::string weaponType, int lvl)
{
	auto owner = static_cast<ComponentManager*>(GetOwner());
	auto name = static_cast<IMyGameObject*>(owner->GetOwner())->GetName();
	std::string weaponName(name + Str(bullet_id++));
	// Serialize the game object and set its owner to be the root object.
	if (g_weaponSerializationMap.find(weaponType) != g_weaponSerializationMap.end())
	{
		auto pNewObject = LoadIWeaponFromFile(this, weaponType.c_str(), weaponName);
		// Add game object under the resource manager.
		if (1 == g_pGameObjectResourceManager->AddResource(pNewObject, weaponName))
		{
			std::string msg("Adding resource : '" + std::string(weaponName) + "' has failed.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
		// Set bullet to the current weapon level
		pNewObject->SetUpgradeLevel(lvl);
		return pNewObject;
	}
	else
	{
		std::string msg("File does not provide a valid game object of type: " + std::string(weaponType));
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
	}
}
