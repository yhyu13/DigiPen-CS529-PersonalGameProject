#include "EventManager.hpp"
#include "Common/Entity/IGameObject.hpp"

My::EventManager::EventManager() noexcept
	:
	m_owner(nullptr)
{
}

int My::EventManager::Initialize()
{
	return 0;
}

int My::EventManager::Tick(double deltaTime)
{
	while (!m_eventsQueue.empty())
	{
		auto e = m_eventsQueue.front();
		// Event type is registered
#if USE_EventRegistry
		if (m_eventsRegistry.find(e->GetType()) != m_eventsRegistry.end() && m_eventsRegistry[e->GetType()])
		{
			reinterpret_cast<IGameObject*>(m_owner)->HandleEvent(e);
		}
#else
		reinterpret_cast<IGameObject*>(m_owner)->HandleEvent(e);
#endif
		m_eventsQueue.pop();
	}
	return 0;
}

int My::EventManager::Finalize()
{
	return 0;
}

void My::EventManager::Serialize(std::wostream& wostream)
{
	auto tmp_q(m_eventsQueue); //copy the original queue to the temporary queue
	while (!tmp_q.empty())
	{
		auto q_element = tmp_q.front();
		q_element->Serialize(wostream);
		tmp_q.pop();
	}
}

void My::EventManager::AddEventRegistry(EventType type) 
{ 
	m_eventsRegistry[type] = true; 
}

void My::EventManager::AddEvent(std::weak_ptr<Event> e) 
{ 
	std::shared_ptr<Event> _e = e.lock();
	m_eventsQueue.push(_e); 
}

void* My::EventManager::GetOwner() noexcept 
{ 
	return m_owner; 
}

void My::EventManager::SetOwner(void* owner) noexcept 
{ 
	m_owner = owner; 
}
