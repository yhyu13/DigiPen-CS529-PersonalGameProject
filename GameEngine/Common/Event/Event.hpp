#pragma once

#include <vector>
#include <memory>
#include "EventType.h"

namespace My {

	class Event
	{
	public:
		Event() = delete;
		explicit Event(EventType type);
		virtual ~Event();
		virtual void Serialize(std::wostream& wostream);
		EventType GetType();
		void AddSubscriber(void* sub);
		std::vector<void*>& GetSubscriber();
	private:
		EventType m_type;
		std::vector<void*> subs;
	};

	namespace Events
	{
		class ComponentHitEvent : public Event
		{
		public:
			ComponentHitEvent() = delete;
			explicit ComponentHitEvent(void* _other) : Event(EventType::ON_COMPONENTHIT), other(_other) {};
		public:
			void* other;
		};

		class TeleportEvent : public Event
		{
		public:
			TeleportEvent() : Event(EventType::ON_TELEPORT) {};
		};

		class SetVelocityEvent : public Event
		{
		public:
			SetVelocityEvent() = delete;
			explicit SetVelocityEvent(float vx, float vy) : Event(EventType::ON_SET_VELOCITY),vx(vx),vy(vy)  {};
		public: 
			float vx;
			float vy;
		};
	}
}