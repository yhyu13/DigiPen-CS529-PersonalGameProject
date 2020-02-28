#pragma once

#include "Common/InputManager.hpp"
#include "Common/Component/Logic/Controller.hpp"

#define PLAYER_SPEED 600.0f

namespace My
{
	namespace Component
	{
		class PlayerController : public IComponent
		{
		public:
			PlayerController() = delete;
			explicit PlayerController(void* owner, std::string name);
			virtual void Update(double deltaTime) override;
			virtual void Serialize(std::wostream& wostream) override;
		};
	}

	class PlayerControllerCreator : public IComponentCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IComponent* Create(void* owner = nullptr, MemoryPool* pool = nullptr, std::string name = "PlayerController0") override
		{
			if (pool)
			{
				return pool->New<Component::PlayerController>(owner, name);
			}
			else
			{
				return new Component::PlayerController(owner, name);
			}
		}
	};
}