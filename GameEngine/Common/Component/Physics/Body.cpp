#include "Body.hpp"
#include "Common/Utility/Utility.hpp"

namespace My
{
	/* (Optional) Using a custom memory pool to create a allocated space */

	IComponent* BodyCreator::Create(void* owner, MemoryPool* pool, std::string name)
	{
		if (pool)
		{
			return pool->New<Component::Body>(owner, name);
		}
		else
		{
			return new Component::Body(owner, name);
		}
	}
}

My::Component::Body::Body(void* owner, std::string name) 
	: 
	IComponent(COMPONENT_TYPE::BODY, owner, name), 
	b_simulated(false), 
	m_Mass(0.0f), 
	m_InvMass(0.0f), 
	m_shape(nullptr), 
	m_angle(0.0f)
{
	Vector2DZero(&m_position);
	Vector2DZero(&m_prev_position);
	Vector2DZero(&m_velocity);
	Vector2DZero(&m_acceleration);
	Vector2DZero(&m_force);
}

My::Component::Body::~Body()
{
	if (m_shape) g_pMemoryManager->Delete(m_shape);
}

void My::Component::Body::Update(double deltaTime)
{
}

void My::Component::Body::PhysicsUpdate(double deltaTime)
{
	std::lock_guard<std::mutex>lock(m_mutex);

	float t = static_cast<float>(deltaTime);
	if (m_Mass != 0.0f && b_simulated)
	{
		Vector2DScale(&m_acceleration, &m_force, m_Mass);				// Update acceleration	
	}
	Vector2DScaleAdd(&m_velocity, &m_acceleration, &m_velocity, t);	// Update velocity

	Vector2DSet(&m_prev_position, &m_position);						// Update prev position
	Vector2DScaleAdd(&m_position, &m_velocity, &m_position, t);		// Update postion

	Vector2DZero(&m_force);											// Reset force to zero at the end of each update.
																	// The force on body should be modified by other body component && various logical event such as collision.
}

void My::Component::Body::Serialize(std::ifstream& fstream)
{
	for (std::string line; getline(fstream, line);)
	{
		if (line[0] == ']')
		{
			return;
		}

		if (6 == sscanf_s(line.c_str(), "p (%f,%f) v (%f,%f) a (%f,%f)", &m_position.x, &m_position.y, &m_velocity.x, &m_velocity.y, &m_acceleration.x, &m_acceleration.y))
		{
			continue;
		}

		if (1 == sscanf_s(line.c_str(), "m %f", &m_Mass))
		{
			if (m_Mass > 0.0f) m_InvMass = 1.0f / m_Mass;
			continue;
		}

		char shapeType[256] = { 0 };
		if (1 == sscanf_s(line.c_str(), "!%s", &shapeType, 256))
		{
			// Serialize the game object component
			if (g_shapeSerializationMap.find(shapeType) != g_shapeSerializationMap.end())
			{
				if (m_shape) g_pMemoryManager->Delete(m_shape);
				m_shape = g_shapeCreatorMap[g_shapeSerializationMap[shapeType]]->Create(this, g_pMemoryManager.get());
				m_shape->Serialize(fstream);
			}
			else
			{
				print_fstream(fstream);
				std::string msg("Body component does not provide a valid shape type: " + std::string(shapeType) + ".");
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
			}
		}
	}
}

void My::Component::Body::Serialize(std::wostream& wostream)
{
	wostream << "[Component: Body, ";
	wostream << "Mass:" << m_Mass << "]";
}

void My::Component::Body::Copy(const Body* body)
{
	Vector2DSet(&m_position, &body->m_position);
	Vector2DSet(&m_prev_position, &body->m_prev_position);
	Vector2DSet(&m_velocity, &body->m_velocity);
	Vector2DSet(&m_acceleration, &body->m_acceleration);
	Vector2DSet(&m_force, &body->m_force);
	m_angle = body->m_angle;
}
