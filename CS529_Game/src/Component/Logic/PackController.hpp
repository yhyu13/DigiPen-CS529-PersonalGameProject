#pragma once

#include "Common/InputManager.hpp"
#include "Common/Component/Logic/Controller.hpp"
#include "Common/Geommath/2D/Vector2D.hpp"

#define PACK_SPEED_LOW 100.0f
#define PACK_SPEED_HIGH 200.0f

namespace My
{
	namespace Component
	{
		class PackController : public IComponent
		{
		public:
			PackController() = delete;
			explicit PackController(void* owner, std::string name);
			virtual void Update(double deltaTime) override;
			virtual void Serialize(std::wostream& wostream) override;
		private:
			Vector2D m_velocity;
		};
	}

	class PackControllerCreator : public IComponentCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IComponent* Create(void* owner = nullptr, MemoryPool* pool = nullptr, std::string name = "PackController0") override
		{
			if (pool)
			{
				return pool->New<Component::PackController>(owner, name);
			}
			else
			{
				return new Component::PackController(owner, name);
			}
		}
	};
}