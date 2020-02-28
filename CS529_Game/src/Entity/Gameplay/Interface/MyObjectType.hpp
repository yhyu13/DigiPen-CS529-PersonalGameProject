#pragma once
#include <string>
#include <map>
#include <memory>

#include "Common/Geommath/2D/Math.hpp"


// Player 
#define COLLISION_HP_REDUCE 2.0f
#define PLAYER_HP_RESTORE 100.0f
#define PLAYER_LOCKHP_PERIOD 4.0
#define MAX_WEAPON_LVL 3
#define WINNING_HEAD_COUNT 3+3+3+5+6+6+1+6+6+3+3

// Pack life time
#define PACK_LIFETIME 6.0
// Pack drop index
#define PACK_HP_INDEX 1
#define PACK_UPGRADE_INDEX 2
#define PACK_SHIELD_INDEX 3
// Pack drop threshold
#define PACK_HP_THRESHOLD .1
#define PACK_UPGRADE_THRESHOLD .2
#define PACK_SHIELD_THRESHOLD .3


namespace My
{
	enum class MYGAMEOBJECT_TYPE
	{
		BACKGROUND = 0,
		PLAYER,
		UFO,
		MINIBOSS,
		BOSS,

		Item_HP,
		Item_SHIELD,
		Item_UPGRADE,

		WEAPON,

		UI_Button,
		UI_Volume_Button,
		NUM
	};
}

