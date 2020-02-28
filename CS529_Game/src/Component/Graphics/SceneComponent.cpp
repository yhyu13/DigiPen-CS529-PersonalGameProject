#include "SceneComponent.hpp"
#include "Common/Component/ComponentManager.hpp"
#include "Common/Component/Physics/Body.hpp"
#include "Common/Entity/IGameObject.hpp"

#include "src/Component/Graphics/Sprite.hpp"
#include "src/Platform/Win32_SDL/WinSDLOpenGLApplication.hpp"
#include "src/Entity/Gameplay/UI/Button.hpp"

My::Component::SceneComponent::SceneComponent(void* owner, std::string name) : IComponent(COMPONENT_TYPE::SCENECOMPONENT, owner, name)
{
}

My::Component::SceneComponent::~SceneComponent()
{
}

void My::Component::SceneComponent::Update(double deltaTime)
{
}

void My::Component::SceneComponent::Serialize(std::wostream& wostream)
{
	wostream << "[Component : SceneComponent] ";
}

void My::Component::SceneComponent::Draw()
{
	auto name = static_cast<IMyGameObject*>(static_cast<ComponentManager*>(GetOwner())->GetOwner())->GetName();

	if (name.find("Background") != std::string::npos)
	{
		DrawBackground();
	}
	else if (name.find("PauseMenu") != std::string::npos)
	{
		DrawPauseMenu();
	}
	else
	{
		DrawObject();
	}
}

void My::Component::SceneComponent::DrawObject()
{
	auto owner = static_cast<ComponentManager*>(GetOwner());
	auto temp_sprite = owner->GetComponent(COMPONENT_TYPE::SPRITE);
	auto p_spriteCom = static_cast<Component::Sprite*>(temp_sprite);
	auto p_App = static_cast<WinSDLOpenGLApplication*>(g_pBaseApplication.get());
	SDL_RenderCopyEx(p_App->GetRenderer(), p_spriteCom->m_sprite, nullptr, &p_spriteCom->m_rcSprite, p_spriteCom->m_degree, nullptr, SDL_FLIP_NONE);
}

void My::Component::SceneComponent::DrawPauseMenu()
{
	auto owner = static_cast<ComponentManager*>(GetOwner());
	auto button = static_cast<Entity::UI_Button*>(owner->GetOwner());

	if (BaseApplication::IsPuased())
	{
		button->SetIsShown(true);
		auto temp_sprite = owner->GetComponent(COMPONENT_TYPE::SPRITE);
		auto p_spriteCom = static_cast<Component::Sprite*>(temp_sprite);
		auto p_App = static_cast<WinSDLOpenGLApplication*>(g_pBaseApplication.get());
		SDL_RenderCopyEx(p_App->GetRenderer(), p_spriteCom->m_sprite, nullptr, &p_spriteCom->m_rcSprite, p_spriteCom->m_degree, nullptr, SDL_FLIP_NONE);
	}
	else
	{
		button->SetIsShown(false);
	}
}

	

void My::Component::SceneComponent::DrawBackground()
{
	auto owner = reinterpret_cast<ComponentManager*>(GetOwner());
	auto temp_sprite = owner->GetComponent(COMPONENT_TYPE::SPRITE);
	auto temp_body = owner->GetComponent(COMPONENT_TYPE::BODY);
	auto p_App = reinterpret_cast<WinSDLOpenGLApplication*>(g_pBaseApplication.get());

	if (temp_sprite && temp_body && p_App)
	{
		auto p_spriteCom = reinterpret_cast<Component::Sprite*>(temp_sprite);
		auto p_bodyCom = reinterpret_cast<Component::Body*>(temp_body);

		int SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT;
		SDL_GetWindowSize(p_App->GetWindow(), &SCREEN_WIDTH, &SCREEN_HEIGHT);
		SPRITE_WIDTH = p_spriteCom->m_rcSprite.w;
		SPRITE_HEIGHT = p_spriteCom->m_rcSprite.h;

		// Repeater mechanism, reset backgroud sprite to the origin
		{
			std::lock_guard<std::mutex> lock(p_bodyCom->m_mutex);
			p_bodyCom->m_position.x = fmodf(p_bodyCom->m_position.x, static_cast<float>(SPRITE_WIDTH));
			p_bodyCom->m_position.y = fmodf(p_bodyCom->m_position.y, static_cast<float>(SPRITE_HEIGHT));
		}

		SDL_Rect  rcSurface = p_spriteCom->m_rcSprite;
		/* draw the background */
		for (int x = -1; x < SCREEN_WIDTH / SPRITE_WIDTH + 2; x++)
		{
			for (int y = -1; y < SCREEN_HEIGHT / SPRITE_HEIGHT + 2; y++)
			{
				rcSurface.x = x * SPRITE_WIDTH + p_spriteCom->m_rcSprite.x;
				rcSurface.y = y * SPRITE_HEIGHT + p_spriteCom->m_rcSprite.y;
				SDL_RenderCopy(p_App->GetRenderer(), p_spriteCom->m_sprite, nullptr, &rcSurface);
			}
		}
	}
}
