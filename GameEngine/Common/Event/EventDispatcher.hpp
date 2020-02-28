#pragma once
#include <vector>
#include "Common/Event/EventManager.hpp"
#include "Common/Entity/GameObjectResourceManager.hpp"



namespace My
{
	class EventDispatcher : public IRunTimeModule
	{
	public:
		EventDispatcher() noexcept;
		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;

		virtual int Initialize() override;
		virtual int Tick(double deltaTime) override;
		virtual int Finalize() override;
		virtual void Serialize(std::wostream& wostream) override;
		void Clear();
		void BroadcastToAll(double wait_time, std::weak_ptr<Event> e);
		void DispatchToGameObject(double wait_time, std::weak_ptr<Event> e, IGameObject* object);
		void DispatchToSubscribers(double wait_time, std::weak_ptr<Event> e);

	public:
		typedef std::pair<double, std::pair<std::shared_ptr<Event>, IGameObject*>> event_object_type;
		/* Support delayed events */
		std::vector<event_object_type> m_disptachList;
	};
	// TODO : Initialize in your application
	extern std::shared_ptr<EventDispatcher> g_pEventDispatcher;
}