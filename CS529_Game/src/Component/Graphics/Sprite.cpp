#include "Sprite.hpp"
#include "Common/Component/ComponentManager.hpp"
#include "Common/Component/Physics/Body.hpp"
#include "src/Platform/Win32_SDL/WinSDLOpenGLApplication.hpp"
#include "Common/Utility/Utility.hpp"

My::Component::Sprite::Sprite(void* owner, std::string name) 
	: 
	IComponent(COMPONENT_TYPE::SPRITE, owner, name),
	m_sprite(nullptr),
	m_degree(0.0f),
	flash_period(0.0),
	flash_period_timer(0.0),
	m_currentSpriteName("")
{ 
	m_rcSprite.x = 0; 
	m_rcSprite.y = 0; 
	m_rcSprite.w = 0;
	m_rcSprite.h = 0;
	m_spriteMap["nullptr"] = std::make_pair(nullptr, m_rcSprite);
}

My::Component::Sprite::~Sprite()
{
	for (auto it = m_spriteMap.begin(); it != m_spriteMap.end(); ++it)
	{
		SDL_DestroyTexture(it->second.first);
	}
}

void My::Component::Sprite::Update(double deltaTime)
{
	// Flash to default sprite
	if (flash_period != 0.0 && (flash_period_timer+=deltaTime) >= flash_period * 0.5)
	{
		if (m_currentSpriteName.compare("Hit") == 0)
		{
			SetCurrentSpriteByName("Default");
		}
	}

	auto owner = static_cast<ComponentManager*>(GetOwner());
	auto temp = owner->GetComponent(COMPONENT_TYPE::BODY);
	auto p_App = static_cast<WinSDLOpenGLApplication*>(g_pBaseApplication.get());
	if (temp && p_App)
	{
		int SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE_W, SPRITE_H;
		SDL_GetWindowSize(p_App->GetWindow(), &SCREEN_WIDTH, &SCREEN_HEIGHT);

		m_rcSprite.w = m_spriteMap[m_currentSpriteName].second.w;
		m_rcSprite.h = m_spriteMap[m_currentSpriteName].second.h;

		if (m_rcSprite.w == 0 && m_rcSprite.h == 0)
		{
			SDL_QueryTexture(m_sprite, nullptr, nullptr, &SPRITE_W, &SPRITE_H);
			m_rcSprite.w = SPRITE_W;
			m_rcSprite.h = SPRITE_H;
		}
		else
		{
			SPRITE_W = m_rcSprite.w;
			SPRITE_H = m_rcSprite.h;
		}

		auto p_transformCom = static_cast<Component::Body*>(temp);
		{
			std::lock_guard<std::mutex> lock(p_transformCom->m_mutex);
			m_rcSprite.x = static_cast<int>(p_transformCom->m_position.x) - SPRITE_W / 2;
			m_rcSprite.y = static_cast<int>(p_transformCom->m_position.y) - SPRITE_H / 2;
			m_degree = p_transformCom->m_angle / RAD;
		}
	}
}

void My::Component::Sprite::Serialize(std::ifstream& fstream)
{
	for (std::string line; getline(fstream, line);)
	{
		char spritePath[256] = { 0 };
		char spriteName[256] = { 0 };
		SDL_Rect rc;
		rc.x = 0;
		rc.y = 0;
		rc.w = 0;
		rc.h = 0;
		if (1 == sscanf_s(line.c_str(), "flash_period %lf", &flash_period))
		{
		}
		else if (2 <= sscanf_s(line.c_str(), "~%s %s %d %d", spritePath, 256, spriteName, 256, &rc.w, &rc.h))
		{
			std::string path(spritePath);

			if (m_spriteMap.find(spriteName) != m_spriteMap.end())
			{
				// If m_sprite exists, free it for updating.
				SDL_DestroyTexture(m_spriteMap[spriteName].first);
			}
			auto p_App = static_cast<WinSDLOpenGLApplication*>(g_pBaseApplication.get());
			auto* bmpTex = IMG_LoadTexture(p_App->GetRenderer(), spritePath);
			if (!bmpTex)
			{
				print_fstream(fstream);
				std::string msg("Sprite path " + path + " can not be loaded. \nIMG error : " + IMG_GetError());
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
			}
			m_currentSpriteName = spriteName;
			m_rcSprite = rc;
			m_spriteMap[spriteName] = std::make_pair(bmpTex, rc);
			m_sprite = bmpTex;
		}
		else if (line[0] == ']')
		{
			return;
		}
		else
		{
			print_fstream(fstream);
			std::string msg("Sprite serialization has failed at line " + line + ".");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
	}
}

void My::Component::Sprite::Serialize(std::wostream& wostream)
{
	wostream << "[Component : Sprite ";
	for (auto it = m_spriteMap.begin(); it != m_spriteMap.end(); ++it)
	{
		wostream << str2wstr("sprite : " + it->first + " ");
	}
	wostream << "] ";
}

void My::Component::Sprite::StartHitFlash()
{
	// Flash to hit sprite
	if (flash_period_timer >= flash_period)
	{
		flash_period_timer = 0.0;
		SetCurrentSpriteByName("Hit");
	}
}

void My::Component::Sprite::SetCurrentSpriteByName(std::string name)
{
	if (m_currentSpriteName.compare(name) != 0)
	{
		if (m_spriteMap.find(name) != m_spriteMap.end())
		{
			m_sprite = m_spriteMap[name].first;
			m_currentSpriteName = name;
		}
		else
		{
			std::string msg("This sprite component does not contain " + name + ".");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
	}	
}
