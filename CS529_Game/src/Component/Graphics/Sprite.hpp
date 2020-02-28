#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>
#include "Common/Component/Graphics/Sprite.hpp"

namespace My
{
	namespace Component
	{
		class Sprite : public IComponent
		{
		public:
			Sprite() = delete;
			explicit Sprite(void* owner, std::string name);
			virtual ~Sprite();
			virtual void Update(double deltaTime) override;
			virtual void Serialize(std::ifstream& fstream) override;
			virtual void Serialize(std::wostream& wostream) override;

			/* Flash between default and hit sprite */
			void StartHitFlash();

			void SetCurrentSpriteByName(std::string name);

		public:
			SDL_Texture* m_sprite;
			SDL_Rect  m_rcSprite;
			/* degree is in degree */
			float m_degree;
		private:
			std::string m_currentSpriteName;
			double flash_period;
			double flash_period_timer;
			// Sprites managed by this component under different names.
			std::map<std::string, std::pair<SDL_Texture*, SDL_Rect>> m_spriteMap;
		};
	}

	class SpriteCreator : public IComponentCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IComponent* Create(void* owner = nullptr, MemoryPool* pool = nullptr, std::string name = "Sprite0") override
		{
			if (pool)
			{
				return pool->New<Component::Sprite>(owner, name);
			}
			else
			{
				return new Component::Sprite(owner, name);
			}
		}
	};
}

