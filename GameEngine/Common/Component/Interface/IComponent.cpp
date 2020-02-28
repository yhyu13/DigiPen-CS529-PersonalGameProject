#include "IComponent.hpp"

My::IComponent::IComponent(COMPONENT_TYPE type, void* owner, std::string name) noexcept : m_type(type), m_owner(owner), m_active(true), m_name(name) {}

My::IComponent::~IComponent() {}

void My::IComponent::Serialize(std::ifstream& fstream)
{
	for (std::string line; getline(fstream, line);)
	{
		if (line[0] == ']')
			return;
	}
}

My::COMPONENT_TYPE My::IComponent::GetType() noexcept { return m_type; }

void My::IComponent::SetName(std::string name) noexcept
{
	m_name = name;
}

const std::string& My::IComponent::GetName() const noexcept
{
	return m_name;
}

void* My::IComponent::GetOwner() const noexcept { return m_owner; }

bool My::IComponent::IsActive() noexcept { return m_active; }

void My::IComponent::SetActive(bool v) noexcept { m_active = v; }

void My::IComponent::SetOwner(void* owner) noexcept { m_owner = owner; }