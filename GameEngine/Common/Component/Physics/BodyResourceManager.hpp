#pragma once
#include "Body.hpp"

namespace My {
	class BodyResourceManager : public ResourceManager<Component::Body>
	{
	public:
		virtual int Initialize() override;
		virtual int PhysicsUpdate(double deltaTime);
		virtual int CollisionUpdate(double deltaTime);
	private:
		bool (*Shape2DCollisionFunction[static_cast<size_t>(SHAPE_TYPE::NUM)][static_cast<size_t>(SHAPE_TYPE::NUM)])(Component::Body* body1, Component::Body* body2);
	};

	// TODO : Initialize in your application
	extern std::shared_ptr<BodyResourceManager> g_pBodyResourceManager;

	namespace Collision
	{
		bool Circle2Circle(Component::Body* body1, Component::Body* body2);
		bool Circle2Rectangle(Component::Body* body1, Component::Body* body2);
		bool Rectangle2Circle(Component::Body* body1, Component::Body* body2);
		bool Rectangle2Rectangle(Component::Body* body1, Component::Body* body2);
	}
}