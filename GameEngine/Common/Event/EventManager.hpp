#pragma once
#include <map>
#include <queue>
#include "Event.hpp"
#include "Common/Interface/IRunTimeModule.hpp"

/* Game objects must register a event type in order for the event manager to process it if USE_EventRegistry = 1 */
#define USE_EventRegistry 0

namespace My
{
	class EventManager : public IRunTimeModule
	{
	public:
		EventManager() noexcept;
		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;
		virtual void Serialize(std::wostream& wostream) override;

		/* Marking the event type as registered to be true */
		void AddEventRegistry(EventType type);
		/* Add certain event that will be managed by the event manager */
		void AddEvent(std::weak_ptr<Event> e);
		/* Owner is a IGameObject */
		void* GetOwner() noexcept;
		void SetOwner(void* owner) noexcept;

	private:
		// Owner is an entity (e.g. IGameObject)
		void* m_owner;
		std::queue<std::shared_ptr<Event>> m_eventsQueue;
		std::map<EventType, bool> m_eventsRegistry;
	};

}