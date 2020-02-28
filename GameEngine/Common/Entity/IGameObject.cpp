#include "IGameObject.hpp"

My::IGameObject::IGameObject(GAMEOBJECT_TYPE type, void* owner, std::string name) noexcept
	:
	m_type(type), 
	m_owner(owner), 
	m_active(true),
	m_gc(false),
	m_name(name)
{
	m_eventManager.SetOwner(this);
	m_componentManager.SetOwner(this);
}

int My::IGameObject::Initialize()
{
	return 0;
}

int My::IGameObject::Tick(double deltaTime)
{
	m_eventManager.Tick(deltaTime);
	m_componentManager.Tick(deltaTime);
	return 0;
}

int My::IGameObject::Finalize()
{
	return 0;
}

int My::IGameObject::Draw() 
{ 
	return 0; 
}

void My::IGameObject::Serialize(std::wostream& wostream)
{
	wostream << str2wstr("Game object : " + m_name  + "\n");
	m_componentManager.Serialize(wostream);
	wostream << "\n";
	for (auto& child : m_children)
	{
		child->Serialize(wostream);
	}
}

void My::IGameObject::AddEvent(std::weak_ptr<Event> e) 
{ 
	m_eventManager.AddEvent(e); 
}

My::IComponent* My::IGameObject::AddComponent(COMPONENT_TYPE type)
{
	return m_componentManager.AddComponent(type);
}

My::IComponent* My::IGameObject::GetComponent(COMPONENT_TYPE type)
{
	return m_componentManager.GetComponent(type);
}

std::vector<My::IGameObject*>& My::IGameObject::GetChildren() 
{ 
	return m_children; 
}

void My::IGameObject::AddChildren(IGameObject* child)
{
	m_children.push_back(child);
}

std::map<std::string, std::function<void(void)>>& My::IGameObject::GetCallBacks()
{
	return m_callBacks;
}

void My::IGameObject::AddCallBacks(std::string name, std::function<void(void)> func)
{
	m_callBacks[name] = func;
}

My::GAMEOBJECT_TYPE My::IGameObject::GetType() const noexcept { return m_type; }

void My::IGameObject::SetName(std::string name) noexcept
{
	m_name = name;
}

const std::string& My::IGameObject::GetName() const noexcept
{
	return m_name;
}

void My::IGameObject::SetOwner(void* owner) noexcept 
{
	m_owner = owner; 
}

void* My::IGameObject::GetOwner() const noexcept 
{ 
	return m_owner; 
}

bool My::IGameObject::IsActive() const noexcept 
{ 
	return m_active; 
}

void My::IGameObject::SetActive(bool v) noexcept 
{ 
	m_active = v; 
}

 bool My::IGameObject::IsGC() const noexcept
{
	return m_gc;
}

 void My::IGameObject::SetGCToTrue() noexcept
{
	m_active = false;
	m_gc = true;
}

My::RootObject::RootObject() : IGameObject(GAMEOBJECT_TYPE::NUM, nullptr, "RootObject") {}
int My::RootObject::Initialize() { return 0; }
int My::RootObject::Tick(double deltaTime) { for (auto& child : m_children) { child->Tick(deltaTime); } return 0; }
int My::RootObject::Draw() { for (auto& child : m_children) { child->Draw(); } return 0; }
int My::RootObject::Finalize() { return 0; }
void My::RootObject::Serialize(std::wostream& wostream) { for (auto& child : m_children) { child->Serialize(wostream); } }