#pragma once
#include <vector>
#include "Common/Event/Event.hpp"

namespace My
{
	namespace Events
	{
		class DeathEvent : public Event
		{
		public:
			DeathEvent() = delete;
			explicit DeathEvent(void* _self) : Event(EventType::ON_DEATH), other(_self) {};
		public:
			void* other;
		};

		class LockHPEvent : public Event
		{
		public:
			LockHPEvent() : Event(EventType::ON_LOCKHP) {};
		};

		class UnLockHPEvent : public Event
		{
		public:
			UnLockHPEvent() : Event(EventType::ON_UNLOCKHP) {};
		};

		class MouseClickEvent : public Event
		{
		public:
			MouseClickEvent() = delete;
			explicit MouseClickEvent(int _x, int _y) : Event(EventType::ON_MOUSECLICK), x(_x), y(_y) {};
		public:
			int x, y;
		};

		class MouseMotionEvent : public Event
		{
		public:
			MouseMotionEvent() = delete;
			explicit MouseMotionEvent(int _x, int _y) : Event(EventType::ON_MOUSEMOTION), x(_x), y(_y) {};
		public:
			int x, y;
		};
		
		class AISwitchFireModeEvent : public Event
		{
		public:
			AISwitchFireModeEvent() = delete;
			explicit AISwitchFireModeEvent(int _mode) : Event(EventType::ON_TELEPORT), mode(_mode) {};
		public:
			int mode;
		};
	
	}
}