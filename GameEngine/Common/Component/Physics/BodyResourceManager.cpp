#include "BodyResourceManager.hpp"
#include "Common/Utility/Utility.hpp"
#include "Common/Component/ComponentManager.hpp"
#include "Common/Entity/IGameObject.hpp"
#include "Common/Event/Event.hpp"

int My::BodyResourceManager::Initialize()
{
	Shape2DCollisionFunction[static_cast<size_t>(SHAPE_TYPE::CIRCLE)][static_cast<size_t>(SHAPE_TYPE::CIRCLE)] = Collision::Circle2Circle;
	Shape2DCollisionFunction[static_cast<size_t>(SHAPE_TYPE::CIRCLE)][static_cast<size_t>(SHAPE_TYPE::RECTANGLE)] = Collision::Circle2Rectangle;
	Shape2DCollisionFunction[static_cast<size_t>(SHAPE_TYPE::RECTANGLE)][static_cast<size_t>(SHAPE_TYPE::CIRCLE)] = Collision::Rectangle2Circle;
	Shape2DCollisionFunction[static_cast<size_t>(SHAPE_TYPE::RECTANGLE)][static_cast<size_t>(SHAPE_TYPE::RECTANGLE)] = Collision::Rectangle2Rectangle;
	return 0;
}

int My::BodyResourceManager::PhysicsUpdate(double deltaTime)
{
	for (auto it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it)
	{
		it->second->PhysicsUpdate(deltaTime);
	}
	return 0;
}

int My::BodyResourceManager::CollisionUpdate(double deltaTime)
{
	if (deltaTime != 0.0)
	{
		for (auto it1 = m_resourceMap.begin(); it1 != m_resourceMap.end(); ++it1)
		{
			auto body_com = it1->second;
			auto actor1 = reinterpret_cast<IGameObject*>(reinterpret_cast<ComponentManager*>(it1->second->GetOwner())->GetOwner());
			if (body_com->IsActive() && actor1->IsActive())
			{
				for (auto it2 = std::next(it1); it2 != m_resourceMap.end(); ++it2)
				{
					auto body_com2 = it2->second;
					auto actor2 = reinterpret_cast<IGameObject*>(reinterpret_cast<ComponentManager*>(it2->second->GetOwner())->GetOwner());
					if (body_com2->IsActive() && actor2->IsActive() && it1->second->m_shape && it2->second->m_shape)
					{
						if (Shape2DCollisionFunction[static_cast<size_t>(it1->second->m_shape->GetType())][static_cast<size_t>(it2->second->m_shape->GetType())](body_com, body_com2))
						{
							// Collision has happened.
							actor1->AddEvent(std::make_shared<Events::ComponentHitEvent>(actor2));
							actor2->AddEvent(std::make_shared<Events::ComponentHitEvent>(actor1));
						}
					}
				}
			}
		}
	}
	return 0;
}

bool My::Collision::Circle2Circle(Component::Body* body1, Component::Body* body2)
{
	auto _shape1 = reinterpret_cast<Collision::Circle*>(body1->m_shape);
	auto _shape2 = reinterpret_cast<Collision::Circle*>(body2->m_shape);
	return StaticCircleToStaticCircle(&body1->m_position, _shape1->m_radius, &body2->m_position, _shape2->m_radius);
}

bool My::Collision::Circle2Rectangle(Component::Body* body1, Component::Body* body2)
{
	auto _shape1 = reinterpret_cast<Collision::Circle*>(body1->m_shape);
	auto _shape2 = reinterpret_cast<Collision::Rectangle*>(body2->m_shape);
	return StaticRectToStaticRect(&body1->m_position, _shape1->m_radius, _shape1->m_radius, &body2->m_position, _shape2->m_width, _shape2->m_height);
}

bool My::Collision::Rectangle2Circle(Component::Body* body1, Component::Body* body2)
{
	return Circle2Rectangle(body2, body1);
}

bool My::Collision::Rectangle2Rectangle(Component::Body* body1, Component::Body* body2)
{
	auto _shape1 = reinterpret_cast<Collision::Rectangle*>(body1->m_shape);
	auto _shape2 = reinterpret_cast<Collision::Rectangle*>(body2->m_shape);
	return StaticRectToStaticRect(&body1->m_position, _shape1->m_width, _shape1->m_height, &body2->m_position, _shape2->m_width, _shape2->m_height);
}
