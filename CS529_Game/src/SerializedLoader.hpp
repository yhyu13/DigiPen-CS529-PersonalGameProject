#pragma once

#include "Common/GfxConfiguration.hpp"
#include "Common/ResourceManager.hpp"
#include "Common/Entity/GameObjectResourceManager.hpp"
#include "Common/Component/Physics/BodyResourceManager.hpp"
#include "Common/Event/EventDispatcher.hpp"
#include "Common/BaseApplication.hpp"
#include "Common/Utility/Utility.hpp"

#include "src/Entity/Gameplay/Interface/IMyGameObject.hpp"

namespace My
{
	// TODO : Initialize in your application
	/* This map is used to assign game object with a call back function (e.g. execute callback on a button's OnClick event)*/
	extern std::map<std::string, std::map<std::string, std::function<void(void)>>> g_globalCallBackSerializationMap;
	extern std::map<std::string, std::ifstream> g_gameObjectConfigurationIfstreamMap;

	/**
	Comment :
	Serialization of level config.
	Sample game object configuration file level1.txt:

	[Screen_width 800 ]
	[Screen_height 600 ]
	[MAX_FPS 60 ]
	*/
	IMyGameObject* LoadIGameObjectFromFile(const char* file_path, std::string name);

	/**
	Comment :
	Serialization of game object.
	Sample game object configuration file Player.txt:

	#Player					// Object type -> initiate object creator
	[Transform					// Component Transform
	p (0,0) v (0,0) a (0,0)		// initiate component creator
	]
	[Sprite						// Component Sprite
	~Player.bmp					// initiate component creator
	]
	[PlayerController			// Component PlayerController
	]
	[SceneComponet				// Component scene (This makes the object visible in the scene)
	]
	*/
	int LoadLevelConfigFromFile(const char* file_path, GfxConfiguration& config);

	/**
	Comment :
	Sample game object configuration file level1.txt:

	#./Resource/Configuration/Background.txt Back
	[Transform
	p(0, 0) v(-200, 0) a(0, 0) // One more comment
	]

	// This is a comment : Create a player object with name "Player"
	#./Resource/Configuration/Player.txt Player // This is a comment, too
	[Transform
	p(100, 100) v(0, 0) a(0,0)
	]

	// This is the third comment : Create a UFO object with name "Enemy1"
	#./Resource/Configuration/UFO.txt Enemy1
	[Transform
	p(300, 100) v(0, 0) a(0, 0) // One more comment
	]

	#./Resource/Configuration/UFO.txt Enemy2
	[Transform
	p(400, 500) v(0, 0) a(0, 0) // One more comment
	]

	#./Resource/Configuration/UFO.txt Enemy3
	[Transform
	p(100, 400) v(0, 0) a(0, 0) // One more comment
	]
	*/
	int LoadLevelObjectFromFile(const char* file_path);
}