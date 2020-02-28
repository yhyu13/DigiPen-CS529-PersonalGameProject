#pragma once

#include "Common/InputManager.hpp"
#include "Common/Component/Logic/Controller.hpp"
#include "Common/Geommath/2D/Vector2D.hpp"
#include "src/Entity/Gameplay/Interface/IWeapon.hpp"

namespace My
{
	namespace Component
	{
		class AIController : public IComponent
		{
		public:
			AIController() = delete;
			explicit AIController(void* owner, std::string name);
			virtual void Update(double deltaTime) override;
			virtual void Serialize(std::wostream& wostream) override;
			virtual void Serialize(std::ifstream& fstream) override;

			void UpdateMotion(double deltaTime);
			void UpdateFire(double deltaTime);
			void SetFireMode(int mode);

			IWeapon* SpawnBullet(std::string weaponType, int lvl);
		public:
			bool switched_event_sent;
		private:
			int motion_mode;
			int motion_mode2;
			int motion_mode3;
			int fire_mode;
			int fire_mode2;
			int fire_mode3;
			uint64_t bullet_id;
			Vector2D m_destination;

			double fire_period;
			double fire_period_timer;

			float m_phase;
			float m_amplitude;
			float m_right;
		};
	}

	class AIControllerCreator : public IComponentCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IComponent* Create(void* owner = nullptr, MemoryPool* pool = nullptr, std::string name = "AIController0") override
		{
			if (pool)
			{
				return pool->New<Component::AIController>(owner, name);
			}
			else
			{
				return new Component::AIController(owner, name);
			}
		}
	};
}