#include <SDL_mixer.h>
#include "Button.hpp"
#include "src/Event/MyEvent.hpp"
#include "src/Component/Graphics/Sprite.hpp"
#include "Common/Utility/Utility.hpp"

int My::Entity::UI_Button::Draw()
{
	if (auto sceneCom = m_componentManager.GetComponent(COMPONENT_TYPE::SCENECOMPONENT))
	{
		static_cast<Component::SceneComponent*>(sceneCom)->Draw();
	}
	return 0;
}

void My::Entity::UI_Button::OnClick(std::weak_ptr<Event> e)
{
	auto temp_sprite = m_componentManager.GetComponent(COMPONENT_TYPE::SPRITE);
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::MouseClickEvent*>(_e.get());
	if (temp_sprite && event)
	{
		auto p_spriteCom = static_cast<Component::Sprite*>(temp_sprite);
		if (IN_RANGE(event->x, p_spriteCom->m_rcSprite.x, p_spriteCom->m_rcSprite.x + p_spriteCom->m_rcSprite.w) &&
			IN_RANGE(event->y, p_spriteCom->m_rcSprite.y, p_spriteCom->m_rcSprite.y + p_spriteCom->m_rcSprite.h))
		{
			DEBUG_PRINT(Str(event->x) + ", " + Str(event->y) + " " + m_name + " button is clicked... ");
			if (m_callBacks["OnClick"])
			{
				m_callBacks["OnClick"]();
				DEBUG_PRINT("And a callback is executed.\n");
			}
			else
			{
				DEBUG_PRINT("But there is no callback to execute.\n");
			}
		}
	}
}

void My::Entity::UI_Button::OnHover(std::weak_ptr<Event> e)
{
	auto temp_sprite = m_componentManager.GetComponent(COMPONENT_TYPE::SPRITE);
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::MouseMotionEvent*>(_e.get());
	if (temp_sprite && event)
	{
		auto p_spriteCom = static_cast<Component::Sprite*>(temp_sprite);
		if (IN_RANGE(event->x, p_spriteCom->m_rcSprite.x, p_spriteCom->m_rcSprite.x + p_spriteCom->m_rcSprite.w) &&
			IN_RANGE(event->y, p_spriteCom->m_rcSprite.y, p_spriteCom->m_rcSprite.y + p_spriteCom->m_rcSprite.h))
		{
			p_spriteCom->SetCurrentSpriteByName("Hover");
		}
		else
		{
			p_spriteCom->SetCurrentSpriteByName("Default");
		}
	}
}

void My::Entity::UI_Button::SetIsShown(bool v)
{
	b_isShown = v;
}

bool My::Entity::UI_Button::GetIsShown()
{
	return b_isShown;
}

int My::Entity::UI_Volume_Button::mode = 0;

int My::Entity::UI_Volume_Button::Tick(double deltaTime)
{
	auto temp_sprite = m_componentManager.GetComponent(COMPONENT_TYPE::SPRITE);
	auto p_spriteCom = static_cast<Component::Sprite*>(temp_sprite);
	switch (mode)
	{
	case 0:
		p_spriteCom->SetCurrentSpriteByName("On");
		break;
	case 1:
		p_spriteCom->SetCurrentSpriteByName("Reduce");
		break;
	case 2:
		p_spriteCom->SetCurrentSpriteByName("Off");
		break;
	default:
		break;
	}
	IMyGameObject::Tick(deltaTime);
	return 0;
}

int My::Entity::UI_Volume_Button::Draw()
{
	if (auto sceneCom = m_componentManager.GetComponent(COMPONENT_TYPE::SCENECOMPONENT))
	{
		static_cast<Component::SceneComponent*>(sceneCom)->Draw();
	}
	return 0;
}

void My::Entity::UI_Volume_Button::OnClick(std::weak_ptr<Event> e)
{
	auto temp_sprite = m_componentManager.GetComponent(COMPONENT_TYPE::SPRITE);
	std::shared_ptr<Event> _e = e.lock();
	auto event = static_cast<Events::MouseClickEvent*>(_e.get());
	if (temp_sprite && event)
	{
		auto p_spriteCom = static_cast<Component::Sprite*>(temp_sprite);
		if (IN_RANGE(event->x, p_spriteCom->m_rcSprite.x, p_spriteCom->m_rcSprite.x + p_spriteCom->m_rcSprite.w) &&
			IN_RANGE(event->y, p_spriteCom->m_rcSprite.y, p_spriteCom->m_rcSprite.y + p_spriteCom->m_rcSprite.h))
		{
			DEBUG_PRINT(Str(event->x) + ", " + Str(event->y) + " " + m_name + " button is clicked... ");
			if (++mode > 2)
			{
				mode = 0;
			}
			switch (mode)
			{
			case 0:
				p_spriteCom->SetCurrentSpriteByName("On");
				Mix_VolumeMusic(MIX_MAX_VOLUME);
				Mix_Volume(-1, MIX_MAX_VOLUME);
				break;
			case 1:
				p_spriteCom->SetCurrentSpriteByName("Reduce");
				Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
				Mix_Volume(-1, MIX_MAX_VOLUME / 4);
				break;
			case 2:
				p_spriteCom->SetCurrentSpriteByName("Off");
				Mix_VolumeMusic(0);
				Mix_Volume(-1, 0);
				break;
			default:
				break;
			}
		}
	}
}
