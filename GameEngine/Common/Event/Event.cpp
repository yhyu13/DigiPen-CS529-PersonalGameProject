#include "Event.hpp"

My::Event::Event(EventType type) : m_type(type) {}

My::Event::~Event() {}

void My::Event::Serialize(std::wostream& wostream) {}

My::EventType My::Event::GetType() { return m_type; }

void My::Event::AddSubscriber(void* sub)
{
	subs.push_back(sub);
}

std::vector<void*>& My::Event::GetSubscriber()
{
	return subs;
}

