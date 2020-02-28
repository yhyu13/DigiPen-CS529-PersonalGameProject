#pragma once
#include <mutex>
#include "Common/Geommath/2D/Math2D.hpp"
#include "Common/Component/Interface/IComponent.hpp"
#include "Common/Geommath/Shape/Shape.hpp"
#include "Common/ResourceManager.hpp"

namespace My
{
	namespace Component
	{
		class Body : public IComponent
		{
		public:
			Body() = delete;
			explicit Body(void* owner, std::string name);
			virtual ~Body();
			virtual void Update(double deltaTime) override;				// Trivial
			virtual void PhysicsUpdate(double deltaTime);
			virtual void Serialize(std::ifstream& fstream) override;
			virtual void Serialize(std::wostream& wostream) override;
			
			void Copy(const Body* body);

		public:
			std::mutex m_mutex;
			Vector2D m_position;
			Vector2D m_prev_position;
			Vector2D m_velocity;
			Vector2D m_acceleration;
			Vector2D m_force;
			/* angle in rad */
			float m_angle;				
			/* Response to physical effect (gravity, reflection) or not */
			bool b_simulated;			
			float m_Mass;
			float m_InvMass;
			Collision::Shape* m_shape;
		};
	}

	class BodyCreator : public IComponentCreator
	{
	public:
		/* (Optional) Using a custom memory pool to create a allocated space */
		virtual IComponent* Create(void* owner = nullptr, MemoryPool* pool = nullptr, std::string name = "Body0") override;
	};

}