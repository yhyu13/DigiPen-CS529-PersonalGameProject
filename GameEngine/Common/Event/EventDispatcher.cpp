#include "EventDispatcher.hpp"
#include "Common/Entity/IGameObject.hpp"
#include "Common/Utility/Utility.hpp"
#include <stack>

My::EventDispatcher::EventDispatcher() noexcept
{
}

int My::EventDispatcher::Initialize()
{
	return 0;
}

int My::EventDispatcher::Tick(double deltaTime)
{
	static std::stack<decltype(m_disptachList.begin())> rm_index;
	for (auto it = m_disptachList.begin(); it != m_disptachList.end(); ++it)
	{
		auto& event_object = (*it);
		if ((event_object.first -= deltaTime) <= 0.0)
		{
			rm_index.push(it);
			auto event = event_object.second.first;
			auto object = event_object.second.second;
			object->AddEvent(event);
		}
	}
	// Erase dispatched events
	while (!rm_index.empty())
	{
		m_disptachList.erase(rm_index.top());
		rm_index.pop();
	}
	return 0;
}

int My::EventDispatcher::Finalize()
{
	return 0;
}

void My::EventDispatcher::Serialize(std::wostream& wostream)
{
	for (size_t i = 0; i < m_disptachList.size(); ++i)
	{
		wostream << m_disptachList[i].first ;
		auto event = m_disptachList[i].second.first;
		auto object = m_disptachList[i].second.second;
		event->Serialize(wostream);
		object->Serialize(wostream);
	}
}

void My::EventDispatcher::Clear()
{
	m_disptachList.clear();
}

void My::EventDispatcher::BroadcastToAll(double wait_time, std::weak_ptr<Event> e)
{
	auto _e = e.lock();
	auto gameObjects = g_pGameObjectResourceManager->GetAllResources();
	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		std::pair<std::shared_ptr<Event>, IGameObject*> event_pair(_e, it->second);
		std::pair<double, std::pair<std::shared_ptr<Event>, IGameObject*>> timed_event_pair(wait_time, event_pair);
		m_disptachList.push_back(timed_event_pair);
	}
}

void My::EventDispatcher::DispatchToGameObject(double wait_time, std::weak_ptr<Event> e, IGameObject* object)
{
	auto _e = e.lock();
	std::pair<std::shared_ptr<Event>, IGameObject*> event_pair(_e, object);
	std::pair<double, std::pair<std::shared_ptr<Event>, IGameObject*>> timed_event_pair(wait_time, event_pair);
	m_disptachList.push_back(timed_event_pair);
}

void My::EventDispatcher::DispatchToSubscribers(double wait_time, std::weak_ptr<Event> e)
{
	auto _e = e.lock();
	for (auto& object : _e->GetSubscriber())
	{
		std::pair<std::shared_ptr<Event>, IGameObject*> event_pair(_e, reinterpret_cast<IGameObject*>(object));
		std::pair<double, std::pair<std::shared_ptr<Event>, IGameObject*>> timed_event_pair(wait_time, event_pair);
		m_disptachList.push_back(timed_event_pair);
	}
}
