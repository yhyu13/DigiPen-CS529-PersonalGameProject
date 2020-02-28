#include <SDL.h>
#include "PlayerController.hpp"
#include "Common/Geommath/2D/Vector2D.hpp"
#include "Common/Component/ComponentManager.hpp"
#include "Common/Component/Physics/Body.hpp"
#include "Common/Event/EventDispatcher.hpp"
#include "Common/Entity/GameObjectResourceManager.hpp"
#include "Common/BaseApplication.hpp"
#include "Common/Utility/Utility.hpp"
#include "src/Component/Gameplay/WeaponarySystem.hpp"
#include "src/Platform/Win32_SDL/WinSDLOpenGLApplication.hpp"
#include "src/Component/Graphics/Sprite.hpp"

My::Component::PlayerController::PlayerController(void* owner, std::string name) 
	: 
	IComponent((COMPONENT_TYPE::PLAYERCONTROLLER), owner, name) 
{
}

void My::Component::PlayerController::Update(double deltaTime)
{
	// Puase by clicking "Esc"
	if (g_pInputManager->KeyIsPressed(SDL_SCANCODE_ESCAPE))
	{
		BaseApplication::TogglePause();
	}

	// Controller logic proceeds iff the game is not paused
	if (!BaseApplication::IsPuased())
	{
		float player_speed = PLAYER_SPEED;

		// Slow time
		if (g_pInputManager->KeyIsDown(SDL_SCANCODE_LSHIFT))
		{
			player_speed *= 0.5f;
		}

		// Slow time
		if (g_pInputManager->KeyIsPressed(SDL_SCANCODE_Z))
		{
			BaseApplication::time_modifier -= 0.5;
		}
		// Fast time
		if (g_pInputManager->KeyIsPressed(SDL_SCANCODE_X))
		{
			BaseApplication::time_modifier += 0.5;
		}

		// Restrict the time slowing effect
		BaseApplication::time_modifier = MAX(BaseApplication::time_modifier, 0.5);
		BaseApplication::time_modifier = MIN(BaseApplication::time_modifier, 1.0);

		// Switch to Lazer
		if (g_pInputManager->KeyIsPressed(SDL_SCANCODE_A))
		{
			auto temp = static_cast<ComponentManager*>(GetOwner())->GetComponent(COMPONENT_TYPE::WEAPONARYSYSTEM);
			if (temp)
			{
				DEBUG_PRINT("Switch to Player_Lazer\n");
				auto p_weapon_com = static_cast<WeaponarySystem*>(temp);
				p_weapon_com->ChangeWeapon(1, "Lazer");
			}
		}
		// Switch to Kinetic Gun
		if (g_pInputManager->KeyIsPressed(SDL_SCANCODE_D))
		{
			auto temp = static_cast<ComponentManager*>(GetOwner())->GetComponent(COMPONENT_TYPE::WEAPONARYSYSTEM);
			if (temp)
			{
				DEBUG_PRINT("Switch to Player_KineticGun\n");
				auto p_weapon_com = static_cast<WeaponarySystem*>(temp);
				p_weapon_com->ChangeWeapon(1, "KineticGun");
			}
		}

		// Transformation
		{
			auto temp = static_cast<ComponentManager*>(GetOwner())->GetComponent(COMPONENT_TYPE::BODY);
			auto p_transformCom = static_cast<Component::Body*>(temp);
			std::lock_guard<std::mutex> lock(p_transformCom->m_mutex);

			// Reset rotation
			p_transformCom->m_angle = 0.0f;

			// Reset velocity
			Vector2DZero(&p_transformCom->m_velocity);
			// Modify velocity by player nput
			if (g_pInputManager->KeyIsDown(SDL_SCANCODE_UP))
			{
				Vector2DAdd(&p_transformCom->m_velocity, &p_transformCom->m_velocity, &Vector2D(0.0, -player_speed));
			}
			if (g_pInputManager->KeyIsDown(SDL_SCANCODE_DOWN))
			{
				Vector2DAdd(&p_transformCom->m_velocity, &p_transformCom->m_velocity, &Vector2D(0.0, player_speed));
			}
			if (g_pInputManager->KeyIsDown(SDL_SCANCODE_LEFT))
			{
				Vector2DAdd(&p_transformCom->m_velocity, &p_transformCom->m_velocity, &Vector2D(-player_speed, 0));
			}
			if (g_pInputManager->KeyIsDown(SDL_SCANCODE_RIGHT))
			{
				Vector2DAdd(&p_transformCom->m_velocity, &p_transformCom->m_velocity, &Vector2D(player_speed, 0));
			}
			// Speed Limit
			if (Vector2DSquareLength(&p_transformCom->m_velocity) > player_speed * player_speed)
			{
				Vector2DNormalize(&p_transformCom->m_velocity, &p_transformCom->m_velocity);
				Vector2DScale(&p_transformCom->m_velocity, &p_transformCom->m_velocity, player_speed);
			}

			// Windows Limit
			auto p_App = static_cast<WinSDLOpenGLApplication*>(g_pBaseApplication.get());
			auto temp_sprite = static_cast<ComponentManager*>(GetOwner())->GetComponent(COMPONENT_TYPE::SPRITE);
			if (temp_sprite && p_App)
			{
				auto sprite_com = static_cast<Component::Sprite*>(temp_sprite);
				int SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE_W, SPRITE_H;
				SDL_GetWindowSize(p_App->GetWindow(), &SCREEN_WIDTH, &SCREEN_HEIGHT);
				SPRITE_W = sprite_com->m_rcSprite.w;
				SPRITE_H = sprite_com->m_rcSprite.h;

				/* collide with edges of screen */
				if (sprite_com->m_rcSprite.x < 0) {
					sprite_com->m_rcSprite.x = 0;
					p_transformCom->m_position.x = static_cast<float>(sprite_com->m_rcSprite.x + SPRITE_W / 2);
				}
				else if (sprite_com->m_rcSprite.x > SCREEN_WIDTH - SPRITE_W) {
					sprite_com->m_rcSprite.x = SCREEN_WIDTH - SPRITE_W;
					p_transformCom->m_position.x = static_cast<float>(sprite_com->m_rcSprite.x + SPRITE_W / 2);
				}
				if (sprite_com->m_rcSprite.y < 0) {
					sprite_com->m_rcSprite.y = 0;
					p_transformCom->m_position.y = static_cast<float>(sprite_com->m_rcSprite.y + SPRITE_H / 2);
				}
				else if (sprite_com->m_rcSprite.y > SCREEN_HEIGHT - SPRITE_H) {
					sprite_com->m_rcSprite.y = SCREEN_HEIGHT - SPRITE_H;
					p_transformCom->m_position.y = static_cast<float>(sprite_com->m_rcSprite.y + SPRITE_H / 2);
				}
			}
		}
	}
}

void My::Component::PlayerController::Serialize(std::wostream& wostream)
{
	wostream << "[Component : PlayerController] ";
}
