#include <SDL.h>
#include "PackController.hpp"
#include "Common/Geommath/2D/Vector2D.hpp"
#include "Common/Component/ComponentManager.hpp"
#include "Common/Component/Physics/Body.hpp"

#include "src/Platform/Win32_SDL/WinSDLOpenGLApplication.hpp"
#include "src/Component/Graphics/Sprite.hpp"

My::Component::PackController::PackController(void* owner, std::string name)
	:
	IComponent((COMPONENT_TYPE::PACKCONTROLLER), owner, name)
{
	Vector2DSet(&m_velocity, ((RAND_I(0,1)==0)?-1.0f:1.0f) * RAND_F(PACK_SPEED_LOW, PACK_SPEED_HIGH), ((RAND_I(0, 1) == 0) ? -1.0f : 1.0f) * RAND_F(PACK_SPEED_LOW, PACK_SPEED_HIGH));
}

void My::Component::PackController::Update(double deltaTime)
{
	// Transformation
	{
		auto temp = static_cast<ComponentManager*>(GetOwner())->GetComponent(COMPONENT_TYPE::BODY);
		auto p_App = static_cast<WinSDLOpenGLApplication*>(g_pBaseApplication.get());
		if (temp && p_App)
		{
			int SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE_W, SPRITE_H;
			SDL_GetWindowSize(p_App->GetWindow(), &SCREEN_WIDTH, &SCREEN_HEIGHT);

			auto p_transformCom = static_cast<Component::Body*>(temp);
			std::lock_guard<std::mutex> lock(p_transformCom->m_mutex);

			// Reset rotation
			p_transformCom->m_angle = 0.0f;

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
					if (m_velocity.x < 0.0)
					{
						m_velocity.x = -m_velocity.x;
					}
				}
				else if (sprite_com->m_rcSprite.x > SCREEN_WIDTH - SPRITE_W) {
					sprite_com->m_rcSprite.x = SCREEN_WIDTH - SPRITE_W;
					p_transformCom->m_position.x = static_cast<float>(sprite_com->m_rcSprite.x + SPRITE_W / 2);
					if (m_velocity.x > 0.0)
					{
						m_velocity.x = -m_velocity.x;
					}
				}
				if (sprite_com->m_rcSprite.y < 0) {
					sprite_com->m_rcSprite.y = 0;
					p_transformCom->m_position.y = static_cast<float>(sprite_com->m_rcSprite.y + SPRITE_H / 2);
					if (m_velocity.y < 0.0)
					{
						m_velocity.y = -m_velocity.y;
					}
				}
				else if (sprite_com->m_rcSprite.y > SCREEN_HEIGHT - SPRITE_H) {
					sprite_com->m_rcSprite.y = SCREEN_HEIGHT - SPRITE_H;
					p_transformCom->m_position.y = static_cast<float>(sprite_com->m_rcSprite.y + SPRITE_H / 2);
					if (m_velocity.y > 0.0)
					{
						m_velocity.y = -m_velocity.y;
					}
				}

				// Reset velocity
				Vector2DSet(&p_transformCom->m_velocity, &m_velocity);
			}
		}
	}
}

void My::Component::PackController::Serialize(std::wostream& wostream)
{
	wostream << "[Component : PackController] ";
}
