#include "HealthPoint.hpp"
#include "Common/Utility/Utility.hpp"

My::Component::HealthPoint::HealthPoint(void* owner, std::string name)
	: 
	IComponent((COMPONENT_TYPE::HEALTHPOINT), owner, name),
	m_value(0.0f),
	m_maxValue(0.0f),
	b_lockHP(false)
{
}

void My::Component::HealthPoint::Update(double deltaTime)
{
}

void My::Component::HealthPoint::Serialize(std::ifstream& fstream)
{
	for (std::string line; getline(fstream, line);)
	{
		if (2 == sscanf_s(line.c_str(), "%f %f", &m_value, &m_maxValue))
		{
			
		}
		else if (line[0] == ']')
		{
			return;
		}
		else
		{
			print_fstream(fstream);
			std::string msg("Sprite serialization has failed at line " + line + ".");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
	}
}

void My::Component::HealthPoint::Serialize(std::wostream& wostream)
{
	wostream << "[Component : HealthPoint] ";
}

void My::Component::HealthPoint::LockHP()
{
	b_lockHP = true;
}

void My::Component::HealthPoint::UnLockHP()
{
	b_lockHP = false;
}

void My::Component::HealthPoint::Restore(float value)
{
	if (!b_lockHP)
	{
		if (m_value < m_maxValue)
		{
			m_value += value;
		}
		else
		{
			m_value = m_maxValue;
		}
	}
}

void My::Component::HealthPoint::Reduce(float value)
{
	if (!b_lockHP)
	{
		if (m_value > 0.0f)
		{
			m_value -= value;
		}
		else
		{
			m_value = 0.0f;
		}
	}
}

bool My::Component::HealthPoint::IsDeplited()
{
	return m_value <= 0.0f;
}

bool My::Component::HealthPoint::IsLockHP()
{
	return b_lockHP;
}
