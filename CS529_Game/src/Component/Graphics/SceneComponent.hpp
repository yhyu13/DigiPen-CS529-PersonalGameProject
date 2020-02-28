#pragma once

#include <SDL.h>
#include "Common/Component/Graphics/SceneComponent.hpp"

namespace My
{
	namespace Component
	{
		class SceneComponent : public IComponent
		{
		public:
			SceneComponent() = delete;
			explicit SceneComponent(void* owner, std::string name);
			virtual ~SceneComponent();
			virtual void Update(double deltaTime) override;
			virtual void Serialize(std::wostream& wostream) override;
			virtual void Draw();

			void DrawBackground();
			void DrawPauseMenu();
			void DrawObject();
		};
	}

	class SceneComponentCreator : public IComponentCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IComponent* Create(void* owner = nullptr, MemoryPool* pool = nullptr, std::string name = "SceneComponent0") override
		{
			if (pool)
			{
				return pool->New<Component::SceneComponent>(owner, name);
			}
			else
			{
				return new Component::SceneComponent(owner, name);
			}
		}
	};
}

