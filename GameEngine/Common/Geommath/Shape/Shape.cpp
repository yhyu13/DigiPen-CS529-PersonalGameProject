#include "Shape.hpp"
#include "Common/Utility/Utility.hpp"

My::Collision::Shape::Shape(SHAPE_TYPE type, void* owner)
	: 
	m_type(type), m_owner(owner), m_active(true) 
{
}

My::Collision::Shape::~Shape() {}

My::SHAPE_TYPE My::Collision::Shape::GetType() const noexcept
{
	return m_type;
}

void My::Collision::Shape::SetOwner(void* owner) noexcept
{
	m_owner = owner;
}

void* My::Collision::Shape::GetOwner() const noexcept
{
	return m_owner;
}

bool My::Collision::Shape::IsActive() const noexcept
{
	return m_active;
}

void My::Collision::Shape::SetActive(bool v) noexcept
{
	m_active = v;
}

My::Collision::Circle::Circle(void* owner) 
	: 
	Shape(SHAPE_TYPE::CIRCLE, owner), m_radius(0.0f)
{
}

void My::Collision::Circle::Serialize(std::ifstream& fstream)
{
	std::string line; getline(fstream, line);
	{
		if (1 == sscanf_s(line.c_str(), "r %f", &m_radius))
		{
			return;
		}
		else
		{
			print_fstream(fstream);
			std::string msg("Shape serialization has failed without providing a valid form for radius: r ~.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
	}
}

My::Collision::Rectangle::Rectangle(void* owner) 
	: 
	Shape(SHAPE_TYPE::RECTANGLE, owner), m_width(0.0f), m_height(0.0f)
{
}

void My::Collision::Rectangle::Serialize(std::ifstream& fstream)
{
	std::string line; getline(fstream, line);
	{
		if (2 == sscanf_s(line.c_str(), "w %f h %f", &m_width, &m_height))
		{
			return;
		}
		else
		{
			print_fstream(fstream);
			std::string msg("Shape serialization has failed without providing a valid form for radius: w ~ h ~.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
	}
}
