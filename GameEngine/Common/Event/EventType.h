#pragma once

namespace My {

	enum class EventType
	{
		ON_COMPONENTHIT = 0,
		ON_DEATH,
		ON_LOCKHP,
		ON_UNLOCKHP,

		ON_TELEPORT,		
		ON_SET_VELOCITY,
		ON_SET_POSITION,
		ON_SET_ACCELERATION,
	
		ON_MOUSECLICK,
		ON_MOUSEMOTION,
		NUM
	};
}