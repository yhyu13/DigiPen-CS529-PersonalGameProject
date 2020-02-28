/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameApplication.cpp
Purpose: implementation file of GameApplication
Language: c++ 11
Platform: win32 x86
Project: CS529_final_project
Author: Hang Yu hang.yu 60001119
Creation date: 10/13/2019
- End Header ----------------------------*/
#include "GameApplication.hpp"

#include "src/Platform/Win32_SDL/WinSDLInputManager.hpp"
#include "src/Platform/Win32_SDL/WinSDLAudioManager.hpp"
#include "src/SerializedLoader.hpp"
#include "src/SceneManager.hpp"

#include "src/Event/MyEvent.hpp"
#include "src/Component/Graphics/Sprite.hpp"
#include "src/Component/Graphics/SceneComponent.hpp"
#include "src/Component/Logic/PlayerController.hpp"
#include "src/Component/Logic/AIController.hpp"
#include "src/Component/Logic/PackController.hpp"
#include "src/Component/Gameplay/HealthPoint.hpp"
#include "src/Component/Gameplay/WeaponarySystem.hpp"

#include "src/Entity/GamePlay/Interface/IMyGameObject.hpp"
#include "src/Entity/Gameplay/UI/Button.hpp"
#include "src/Entity/GamePlay/Objects/Player.hpp"
#include "src/Entity/GamePlay/Objects/UFO.hpp"
#include "src/Entity/GamePlay/Objects/Background.hpp"
#include "src/Entity/GamePlay/Pack/Pack_HP.hpp"
#include "src/Entity/GamePlay/Pack/Pack_Shield.hpp"
#include "src/Entity/GamePlay/Pack/Pack_Upgrade.hpp"
#include "src/Entity/Gameplay/Weapon/Lazer.hpp"
#include "src/Entity/Gameplay/Weapon/LazerBeam.hpp"
#include "src/Entity/Gameplay/Weapon/KineticGun.hpp"
#include "src/Entity/Gameplay/Weapon/KineticGunBullet.hpp"
#include "src/Entity/Gameplay/Weapon/KineticGunExplosion.hpp"
#include "src/Entity/Gameplay/Weapon/Missile.hpp"
#include "src/Entity/Gameplay/Weapon/MissileBullet.hpp"
#include "src/Entity/Gameplay/Weapon/MissileExplosion.hpp"
#include "src/Entity/Gameplay/Weapon/UFOBullet.hpp"

#define RegisterSerialization(map, key, value) map[key] = value
#define RegisterCreator(map, key, value) map[key] = std::make_shared<value>()

using namespace My;

/*
	Variable Initializer.
*/
namespace My
{
	// Initialize Maps
	// Game object serialization
	std::map<std::string, GAMEOBJECT_TYPE> g_gameObjectSerializationMap;
	std::map<GAMEOBJECT_TYPE, std::shared_ptr<IGameObjectCreator>> g_gameObjectCreatorMap;
	// My Game Object
	std::map<std::string, MYGAMEOBJECT_TYPE> g_myGameObjectSerializationMap;
	std::map<MYGAMEOBJECT_TYPE, std::shared_ptr<IMyGameObjectCreator>> g_myGameObjectCreatorMap;
	// Shape(collision) object serialization
	std::map<std::string, SHAPE_TYPE> g_shapeSerializationMap;
	std::map<SHAPE_TYPE, std::shared_ptr<ShapeCreator>> g_shapeCreatorMap;
	// Event object serialization
	std::map<std::string, std::map<std::string, std::function<void(void)>>> g_globalCallBackSerializationMap;
	std::map<std::string, std::ifstream> g_gameObjectConfigurationIfstreamMap;
	// Component object serialization
	std::map<std::string, COMPONENT_TYPE> g_componentSerializationMap;
	std::map<COMPONENT_TYPE, std::shared_ptr<IComponentCreator>> g_componentCreatorMap;
	// Weapon object serialization
	std::map<std::string, WEAPON_TYPE> g_weaponSerializationMap;
	std::map<WEAPON_TYPE, std::shared_ptr<IWeaponCreator>> g_weaponCreatorMap;

	// Initialize scene configuration
	std::shared_ptr<RootObject> g_pRootObject = std::make_shared<RootObject>();
	std::shared_ptr<Scene> g_pScene = std::make_shared<Scene>();
	std::shared_ptr<SceneManager> g_pSceneManager = std::make_shared<SceneManager>();

	// Contains all Game object 
	std::shared_ptr<IGameObjectResourceManager> g_pGameObjectResourceManager = std::make_shared<IGameObjectResourceManager>();
	// Manage all physics components
	std::shared_ptr<BodyResourceManager> g_pBodyResourceManager = std::make_shared<BodyResourceManager>();

	// Initialize global pointers
	std::shared_ptr<InputManager> g_pInputManager = std::make_shared<WinSDLInputManager>();
	std::shared_ptr<MemoryManager> g_pMemoryManager = std::make_shared<MemoryManager>();
	std::shared_ptr<EventDispatcher> g_pEventDispatcher = std::make_shared<EventDispatcher>();
	std::shared_ptr<WinSDLAudioManager> g_pAudioManager = std::make_shared<WinSDLAudioManager>();
	
	// Initialize application
	std::shared_ptr<BaseApplication> g_pBaseApplication = std::make_shared<GameApplication>(g_pScene->m_config);
	std::shared_ptr<GameApplication> g_pApp = std::static_pointer_cast<GameApplication>(g_pBaseApplication);
}

MY_CALL_BACK LoadVictory0()
{
	// Loading game object by serialization
	g_pSceneManager->m_queueLevelConfigPath = "Victory0";
	g_pAudioManager->m_queuechunckPath = "Victory0";
	g_pAudioManager->StopMusic();
}

MY_CALL_BACK LoadDefeat0()
{
	// Loading game object by serialization
	g_pSceneManager->m_queueLevelConfigPath = "Defeat0";
	g_pAudioManager->m_queuechunckPath = "Defeat0";
	g_pAudioManager->StopMusic();
}

MY_CALL_BACK LoadLevel1()
{
	// Loading game object by serialization
	g_pSceneManager->m_queueLevelConfigPath = "Level1";
	g_pSceneManager->m_queueWaveConfigPath = "Level1_Wave1";
	g_pAudioManager->m_queueMusicPath = "Level1";
}

MY_CALL_BACK LoadMenu0()
{
	g_pSceneManager->m_queueLevelConfigPath = "Menu0";
	g_pAudioManager->m_queueMusicPath = "Menu0";
}

MY_CALL_BACK QuitApplication()
{
	BaseApplication::SetQuit(true);
}

MY_CALL_BACK LoadMenuGuide0()
{
	g_pSceneManager->m_queueLevelConfigPath = "MenuGuide0";
}

MY_CALL_BACK ReturnToMenu0_1()
{
	g_pSceneManager->m_queueLevelConfigPath = "Menu0";
}

MY_CALL_BACK Resume()
{
	BaseApplication::TogglePause();
}

MY_CALL_BACK RestartFromPause()
{
	LoadLevel1();
	BaseApplication::TogglePause();
}

MY_CALL_BACK RestartLevel1()
{
	LoadLevel1();
}

MY_CALL_BACK ReturnToMenu0FromPause()
{
	LoadMenu0();
	BaseApplication::TogglePause();
}

MY_CALL_BACK ReturnToMenu0()
{
	LoadMenu0();
}

/*
	Callback Initializer
*/
namespace My
{

	MY_CALL_BACK CreatorInitialization()
	{
		{
			// Level
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1", std::ifstream("./Resource/Level/Level1.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Menu0", std::ifstream("./Resource/Level/Menu0.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "MenuGuide0", std::ifstream("./Resource/Level/MenuGuide0.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Victory0", std::ifstream("./Resource/Level/Victory0.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Defeat0", std::ifstream("./Resource/Level/Defeat0.txt"));

			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave1", std::ifstream("./Resource/Level/Level1_Wave1.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave2", std::ifstream("./Resource/Level/Level1_Wave2.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave3", std::ifstream("./Resource/Level/Level1_Wave3.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave4", std::ifstream("./Resource/Level/Level1_Wave4.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave5", std::ifstream("./Resource/Level/Level1_Wave5.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave6", std::ifstream("./Resource/Level/Level1_Wave6.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave7", std::ifstream("./Resource/Level/Level1_Wave7.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave8", std::ifstream("./Resource/Level/Level1_Wave8.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave9", std::ifstream("./Resource/Level/Level1_Wave9.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave10", std::ifstream("./Resource/Level/Level1_Wave10.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Level1_Wave11", std::ifstream("./Resource/Level/Level1_Wave11.txt"));

			// Audio
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "AudioList", std::ifstream("./Resource/Configuration/Audio/AudioList.txt"));
			// Game objects
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Background", std::ifstream("./Resource/Configuration/Object/Background.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Player", std::ifstream("./Resource/Configuration/Object/Player.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "UFO", std::ifstream("./Resource/Configuration/Object/UFO.txt"));
			// UI
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "UI_Button", std::ifstream("./Resource/Configuration/UI/UI_Button.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "UI_Volume_Button", std::ifstream("./Resource/Configuration/UI/UI_Volume_Button.txt"));
			// Pack
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Pack_HP", std::ifstream("./Resource/Configuration/Pack/Pack_HP.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Pack_Shield", std::ifstream("./Resource/Configuration/Pack/Pack_Shield.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Pack_Upgrade", std::ifstream("./Resource/Configuration/Pack/Pack_Upgrade.txt"));
			// Weapons
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Lazer", std::ifstream("./Resource/Configuration/Weapon/Lazer.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "LazerBeam", std::ifstream("./Resource/Configuration/Weapon/LazerBeam.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "KineticGun", std::ifstream("./Resource/Configuration/Weapon/KineticGun.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "KineticGunBullet", std::ifstream("./Resource/Configuration/Weapon/KineticGunBullet.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "KineticGunExplosion", std::ifstream("./Resource/Configuration/Weapon/KineticGunExplosion.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "Missile", std::ifstream("./Resource/Configuration/Weapon/Missile.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "MissileBullet", std::ifstream("./Resource/Configuration/Weapon/MissileBullet.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "MissileExplosion", std::ifstream("./Resource/Configuration/Weapon/MissileExplosion.txt"));
			RegisterSerialization(g_gameObjectConfigurationIfstreamMap, "UFOBullet", std::ifstream("./Resource/Configuration/Weapon/UFOBullet.txt"));
		}

		{
			std::map<std::string, std::function<void(void)>> Player;
			Player["Defeat"] = &LoadDefeat0;
			Player["Victory"] = &LoadVictory0;
			RegisterSerialization(g_globalCallBackSerializationMap, "002_Player", Player);

			// Main Menu
			std::map<std::string, std::function<void(void)>> UI_Start_Button;
			UI_Start_Button["OnClick"] = &LoadLevel1;
			RegisterSerialization(g_globalCallBackSerializationMap, "UI_Start_Button", UI_Start_Button);
			std::map<std::string, std::function<void(void)>> UI_Quit_Button;
			UI_Quit_Button["OnClick"] = &QuitApplication;
			RegisterSerialization(g_globalCallBackSerializationMap, "UI_Quit_Button", UI_Quit_Button);
			std::map<std::string, std::function<void(void)>> UI_GameGuide_Button;
			UI_GameGuide_Button["OnClick"] = &LoadMenuGuide0;
			RegisterSerialization(g_globalCallBackSerializationMap, "UI_GameGuide_Button", UI_GameGuide_Button);

			// Game Guide Menu
			std::map<std::string, std::function<void(void)>> UI_Guide_MainMenu_Button;
			UI_Guide_MainMenu_Button["OnClick"] = &ReturnToMenu0_1;
			RegisterSerialization(g_globalCallBackSerializationMap, "UI_Guide_MainMenu_Button", UI_Guide_MainMenu_Button);

			// Victory/Defeat Menu
			std::map<std::string, std::function<void(void)>> UI_MainMenu_Button;
			UI_MainMenu_Button["OnClick"] = &ReturnToMenu0;
			RegisterSerialization(g_globalCallBackSerializationMap, "UI_MainMenu_Button", UI_MainMenu_Button);
			std::map<std::string, std::function<void(void)>> UI_Restart_Button;
			UI_Restart_Button["OnClick"] = &RestartLevel1;
			RegisterSerialization(g_globalCallBackSerializationMap, "UI_Restart_Button", UI_Restart_Button);

			// In Game Pause Menu
			std::map<std::string, std::function<void(void)>> UI_PauseMenu_MainMenu_Button;
			UI_PauseMenu_MainMenu_Button["OnClick"] = &ReturnToMenu0FromPause;
			RegisterSerialization(g_globalCallBackSerializationMap, "UI_PauseMenu_MainMenu_Button", UI_PauseMenu_MainMenu_Button);
			std::map<std::string, std::function<void(void)>> UI_PauseMenu_Resume_Button;
			UI_PauseMenu_Resume_Button["OnClick"] = &Resume;
			RegisterSerialization(g_globalCallBackSerializationMap, "UI_PauseMenu_Resume_Button", UI_PauseMenu_Resume_Button);
			std::map<std::string, std::function<void(void)>> UI_PauseMenu_Restart_Button;
			UI_PauseMenu_Restart_Button["OnClick"] = &RestartFromPause;
			RegisterSerialization(g_globalCallBackSerializationMap, "UI_PauseMenu_Restart_Button", UI_PauseMenu_Restart_Button);
		}
		
		{
			// Game Objects
			RegisterSerialization(g_myGameObjectSerializationMap, "UI_Button", MYGAMEOBJECT_TYPE::UI_Button);
			RegisterSerialization(g_myGameObjectSerializationMap, "UI_Volume_Button", MYGAMEOBJECT_TYPE::UI_Volume_Button);
			RegisterSerialization(g_myGameObjectSerializationMap, "Player", MYGAMEOBJECT_TYPE::PLAYER);
			RegisterSerialization(g_myGameObjectSerializationMap, "UFO", MYGAMEOBJECT_TYPE::UFO);
			RegisterSerialization(g_myGameObjectSerializationMap, "Background", MYGAMEOBJECT_TYPE::BACKGROUND);
			RegisterSerialization(g_myGameObjectSerializationMap, "Pack_HP", MYGAMEOBJECT_TYPE::Item_HP);
			RegisterSerialization(g_myGameObjectSerializationMap, "Pack_Shield", MYGAMEOBJECT_TYPE::Item_SHIELD);
			RegisterSerialization(g_myGameObjectSerializationMap, "Pack_Upgrade", MYGAMEOBJECT_TYPE::Item_UPGRADE);
			RegisterCreator(g_myGameObjectCreatorMap, MYGAMEOBJECT_TYPE::UI_Button, UI_ButtonObjectCreator);
			RegisterCreator(g_myGameObjectCreatorMap, MYGAMEOBJECT_TYPE::UI_Volume_Button, UI_Volume_ButtonObjectCreator);
			RegisterCreator(g_myGameObjectCreatorMap, MYGAMEOBJECT_TYPE::PLAYER, PlayerObjectCreator);
			RegisterCreator(g_myGameObjectCreatorMap, MYGAMEOBJECT_TYPE::UFO, UFOObjectCreator);
			RegisterCreator(g_myGameObjectCreatorMap, MYGAMEOBJECT_TYPE::BACKGROUND, BackgroundObjectCreator);
			RegisterCreator(g_myGameObjectCreatorMap, MYGAMEOBJECT_TYPE::Item_HP, Pack_HPObjectCreator);
			RegisterCreator(g_myGameObjectCreatorMap, MYGAMEOBJECT_TYPE::Item_SHIELD, Pack_ShieldObjectCreator);
			RegisterCreator(g_myGameObjectCreatorMap, MYGAMEOBJECT_TYPE::Item_UPGRADE, Pack_UpgradeObjectCreator);

			// Weapons
			RegisterSerialization(g_weaponSerializationMap, "Lazer", WEAPON_TYPE::WEAPON_LAZER);
			RegisterSerialization(g_weaponSerializationMap, "LazerBeam", WEAPON_TYPE::WEAPON_LAZER_BEAM);
			RegisterSerialization(g_weaponSerializationMap, "Missile", WEAPON_TYPE::WEAPON_MISSILE);
			RegisterSerialization(g_weaponSerializationMap, "MissileBullet", WEAPON_TYPE::WEAPON_MISSILE_BULLET);
			RegisterSerialization(g_weaponSerializationMap, "MissileExplosion", WEAPON_TYPE::WEAPON_MISSILE_EXPLOSION);
			RegisterSerialization(g_weaponSerializationMap, "KineticGun", WEAPON_TYPE::WEAPON_KINETICGUN);
			RegisterSerialization(g_weaponSerializationMap, "KineticGunBullet", WEAPON_TYPE::WEAPON_KINETICGUN_BULLET);
			RegisterSerialization(g_weaponSerializationMap, "KineticGunExplosion", WEAPON_TYPE::WEAPON_KINETICGUN_EXPLOSION);
			RegisterSerialization(g_weaponSerializationMap, "UFOBullet", WEAPON_TYPE::UFO_BULLET);
			RegisterCreator(g_weaponCreatorMap, WEAPON_TYPE::WEAPON_LAZER, LazerObjectCreator);
			RegisterCreator(g_weaponCreatorMap, WEAPON_TYPE::WEAPON_LAZER_BEAM, LazerBeamObjectCreator);
			RegisterCreator(g_weaponCreatorMap, WEAPON_TYPE::WEAPON_MISSILE, MissleObjectCreator);
			RegisterCreator(g_weaponCreatorMap, WEAPON_TYPE::WEAPON_MISSILE_BULLET, MissleBulletObjectCreator);
			RegisterCreator(g_weaponCreatorMap, WEAPON_TYPE::WEAPON_MISSILE_EXPLOSION, MissileExplosionObjectCreator);
			RegisterCreator(g_weaponCreatorMap, WEAPON_TYPE::WEAPON_KINETICGUN, KineticGunObjectCreator);
			RegisterCreator(g_weaponCreatorMap, WEAPON_TYPE::WEAPON_KINETICGUN_BULLET, KineticGunBulletObjectCreator);
			RegisterCreator(g_weaponCreatorMap, WEAPON_TYPE::WEAPON_KINETICGUN_EXPLOSION, KineticGunExplosionObjectCreator);
			RegisterCreator(g_weaponCreatorMap, WEAPON_TYPE::UFO_BULLET, UFOBulletObjectCreator);
		}
		
		{
			// Shape
			RegisterSerialization(g_shapeSerializationMap, "Circle", SHAPE_TYPE::CIRCLE);
			RegisterSerialization(g_shapeSerializationMap, "Rectangle", SHAPE_TYPE::RECTANGLE);
			RegisterCreator(g_shapeCreatorMap, SHAPE_TYPE::CIRCLE, CircleCreator);
			RegisterCreator(g_shapeCreatorMap, SHAPE_TYPE::RECTANGLE, RectangleCreator);
		}
		
		{
			// Component
			RegisterSerialization(g_componentSerializationMap, "Sprite", COMPONENT_TYPE::SPRITE);
			RegisterSerialization(g_componentSerializationMap, "SceneComponent", COMPONENT_TYPE::SCENECOMPONENT);
			RegisterSerialization(g_componentSerializationMap, "Transform", COMPONENT_TYPE::TRANSFORM);
			RegisterSerialization(g_componentSerializationMap, "Body", COMPONENT_TYPE::BODY);
			RegisterSerialization(g_componentSerializationMap, "PlayerController", COMPONENT_TYPE::PLAYERCONTROLLER);
			RegisterSerialization(g_componentSerializationMap, "AIController", COMPONENT_TYPE::AICONTROLLER);
			RegisterSerialization(g_componentSerializationMap, "PackController", COMPONENT_TYPE::PACKCONTROLLER);
			RegisterSerialization(g_componentSerializationMap, "HealthPoint", COMPONENT_TYPE::HEALTHPOINT);
			RegisterSerialization(g_componentSerializationMap, "WeaponarySystem", COMPONENT_TYPE::WEAPONARYSYSTEM);
			RegisterCreator(g_componentCreatorMap, COMPONENT_TYPE::SPRITE, SpriteCreator);
			RegisterCreator(g_componentCreatorMap, COMPONENT_TYPE::SCENECOMPONENT, SceneComponentCreator);
			RegisterCreator(g_componentCreatorMap, COMPONENT_TYPE::BODY, BodyCreator);
			RegisterCreator(g_componentCreatorMap, COMPONENT_TYPE::PLAYERCONTROLLER, PlayerControllerCreator);
			RegisterCreator(g_componentCreatorMap, COMPONENT_TYPE::AICONTROLLER, AIControllerCreator);
			RegisterCreator(g_componentCreatorMap, COMPONENT_TYPE::PACKCONTROLLER, PackControllerCreator);
			RegisterCreator(g_componentCreatorMap, COMPONENT_TYPE::HEALTHPOINT, HealthPointCreator);
			RegisterCreator(g_componentCreatorMap, COMPONENT_TYPE::WEAPONARYSYSTEM, WeaponarySystemCreator);
		}
	}

	MY_CALL_BACK PrintPerSecond()
	{
		static Timer printTimer;
		static int m_nbFrames = 0;
		m_nbFrames++;
		if (printTimer.Mark() >= 1.0)
		{
			// Print all objects
			// g_pRootObject->Serialize(std::wcout);
			PRINT(Str(1000.0 / m_nbFrames) + " ms/frame\n");
			m_nbFrames = 0;
			printTimer.Reset();
		}
	}
}

/*
	Implementing
	extern MY_CALL_BACK Initialize();
	extern MY_CALL_BACK GameLoop();
	extern MY_CALL_BACK Finalize();
*/

MY_CALL_BACK My::Initialize()
{
	Run(
		[] {
			srand(time(0));
			// Register all creators/factories
			CreatorInitialization();
			g_pSceneManager->Initialize();
			g_pEventDispatcher->Initialize();
			g_pBaseApplication->Initialize();	/* equivalient to g_pApp->Initialize() as they are base-derived classes */
			g_pInputManager->Initialize();
			g_pBodyResourceManager->Initialize();
			g_pGameObjectResourceManager->Initialize();
			g_pMemoryManager->Initialize();
			// Must init after g_pBaseApplication init
			g_pAudioManager->Initialize();

			LoadMenu0();
		}
	);
}

MY_CALL_BACK My::GameLoop()
{
	Run(
		[] {
			Timer drawTimer;
			while (!BaseApplication::IsQuit())
			{
				// Reset timer
				drawTimer.Reset();
				{
					// Must call first
					BaseApplication::UpdateFrameTime(1.0 / g_pScene->m_config.maxfps);
				}
				
				// Managers update
				{
					// All Objects GC
					g_pGameObjectResourceManager->GC();

					// Process scene loading/unloading
					g_pSceneManager->Tick(BaseApplication::frametime);
					// Process audio loading/unloading
					g_pAudioManager->Tick(BaseApplication::frametime);
					// Process logical event in the game application tick
					g_pBaseApplication->Tick(BaseApplication::frametime);
					// Process input event
					g_pInputManager->Tick(BaseApplication::frametime);

					// Process physics (could run parallelly)
					g_pBodyResourceManager->PhysicsUpdate(BaseApplication::frametime);
					// Process collision (could run parallelly with physics)
					g_pBodyResourceManager->CollisionUpdate(BaseApplication::frametime);

					// Dispatch events
					g_pEventDispatcher->Tick(BaseApplication::frametime);
					// All Objects (events and components) update
					g_pGameObjectResourceManager->Tick(BaseApplication::frametime);
				}

				// Graphics update
				{
					// Prepare drawing
					g_pBaseApplication->BeginDraw();
					// Application drawing
					g_pBaseApplication->OnDraw();
					// Draw all objects
					g_pGameObjectResourceManager->DrawAll();
					// End draw call
					g_pBaseApplication->EndDraw();
				}

				// Print game loop debug
				PrintPerSecond();
				// Busy waiting
				while (drawTimer.Mark() < BaseApplication::static_frametime)
				{
				};

			}
			PRINT("Exit!\n");
		}
	);
}


MY_CALL_BACK My::Finalize()
{
	Run(
		[] {
			g_pAudioManager->Finalize();
			g_pSceneManager->Finalize();
			g_pEventDispatcher->Finalize();
			g_pBodyResourceManager->Finalize();
			g_pGameObjectResourceManager->Finalize();
			g_pMemoryManager->Finalize();
			g_pInputManager->Finalize();
			g_pBaseApplication->Finalize();
		}
	);
}
