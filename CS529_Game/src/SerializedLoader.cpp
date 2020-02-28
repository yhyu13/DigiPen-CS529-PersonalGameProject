#include "SerializedLoader.hpp"

namespace My
{
	/**
	Comment :
	Serialization of level config.
	Sample game object configuration file level1.txt:

	[Screen_width 800 ]
	[Screen_height 600 ]
	[MAX_FPS 60 ]
	*/
	int LoadLevelConfigFromFile(const char* file_path, GfxConfiguration& config)
	{
		std::map<std::string, int> config_dict;
		std::ifstream& input(g_gameObjectConfigurationIfstreamMap[Str(file_path)]);
		if (!input.is_open())
		{
			std::string msg(std::string(file_path) + " could not be opened.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
		else
		{
			for (std::string line; getline(input, line);)
			{
				char key[256] = { 0 };
				int value = 0;
				if (2 == sscanf_s(line.c_str(), "[%s %d", key, 256, &value))
				{
					config_dict[key] = value;
				}
			}
			if (config_dict.find("Screen_width") != config_dict.end())
			{
				config.screenWidth = config_dict["Screen_width"];
			}
			if (config_dict.find("Screen_height") != config_dict.end())
			{
				config.screenHeight = config_dict["Screen_height"];
			}
			if (config_dict.find("MAX_FPS") != config_dict.end())
			{
				config.maxfps = config_dict["MAX_FPS"];
			}
			rewind_fstream(input);
			g_pBaseApplication->SetConfig(config);
			return 0;
		}
	}

	/**
	Comment :
	Load game object from file_path and set it with name
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
	IMyGameObject* LoadIGameObjectFromFile(const char* file_path, std::string name)
	{
		if (g_gameObjectConfigurationIfstreamMap.find(Str(file_path)) == g_gameObjectConfigurationIfstreamMap.end())
		{
			std::string msg("The configuration file of game object " + Str(file_path) + " is not registered.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}

		IMyGameObject* pNewObject = nullptr;
		std::ifstream &input(g_gameObjectConfigurationIfstreamMap[Str(file_path)]);

		if (!input.is_open())
		{
			std::string msg("Can't open object file: " + std::string(file_path) + ".");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
		else
		{
			std::string line; getline(input, line);
			char objectType[256] = { 0 };
			if (1 == sscanf_s(line.c_str(), "#%s", objectType, 256))
			{
				// Serialize the game object and set its owner to be the root object.
				if (g_myGameObjectSerializationMap.find(objectType) != g_myGameObjectSerializationMap.end())
				{
					/* owner = g_pRootObject.get(), pool = g_pMemoryManager.get() */
					pNewObject = g_myGameObjectCreatorMap[g_myGameObjectSerializationMap[objectType]]->Create(nullptr, g_pMemoryManager.get(), name);
					/* Assign call backs */
					if (g_globalCallBackSerializationMap.find(name) != g_globalCallBackSerializationMap.end())
					{
						auto callback_map = g_globalCallBackSerializationMap[name];
						for (auto it = callback_map.begin(); it != callback_map.end(); ++it)
						{
							pNewObject->AddCallBacks(it->first, it->second);
						}
					}
				}
				else
				{
					print_fstream(input);
					std::string msg(std::string(file_path) + " does not provide a valid game object of type: " + std::string(objectType) + " at line : " + line);
					throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
				}
			}
			else
			{
				print_fstream(input);
				std::string msg(std::string(file_path) + " does not contain a valid object type in the form #~ : " + line);
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
			}
			for (std::string line; getline(input, line);)
			{
				char componentType[256] = { 0 };
				char componentName[256] = { 0 };
				if (0 < sscanf_s(line.c_str(), "[%s %s", componentType, 256, componentName, 256))
				{
					// Serialize the game object component
					if (g_componentSerializationMap.find(componentType) != g_componentSerializationMap.end())
					{
						// Add component
						auto component = pNewObject->AddComponent(g_componentSerializationMap[componentType]);
						component->Serialize(input);
						// Set name if given
						if (componentName[0] != 0)
						{
							component->SetName(name + componentName);
						}
						else
						{
							component->SetName(name + component->GetName());
						}
						
						if (g_componentSerializationMap[componentType] == COMPONENT_TYPE::BODY)
						{
							/* Adding body component under the management of the physics resource manager */
							if (1 == g_pBodyResourceManager->AddResource(reinterpret_cast<Component::Body*>(component), name))
							{
								std::string msg("Adding resource : '" + component->GetName() + "' has failed.");
								throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
							}
						}
					}
					else
					{
						print_fstream(input);
						std::string msg(std::string(file_path) + " does not provide a valid component type for object type " + std::string(objectType) + " at line : " + line);
						throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
					}
				}
				else
				{
					print_fstream(input);
					std::string msg(std::string(file_path) + " does not contain a valid component type in the form [~ : " + line);
					throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
				}
			}
			rewind_fstream(input);
			return pNewObject;
		}
	}

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
	int LoadLevelObjectFromFile(const char* file_path)
	{
		std::ifstream& input(g_gameObjectConfigurationIfstreamMap[Str(file_path)]);
		if (!input.is_open())
		{
			std::string msg("Can't open level file: " + std::string(file_path) + ".");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
		else
		{
			for (std::string line; getline(input, line);)
			{
				// Load game object from file
				IMyGameObject* pNewObject = nullptr;
				char objectPath[256] = { 0 };
				char objectName[256] = { 0 };
				if (2 == sscanf_s(line.c_str(), "#%s %s", objectPath, 256, objectName, 256))
				{
					pNewObject = LoadIGameObjectFromFile(objectPath, objectName);
					// Add game object under the resource manager.
					if (1 == g_pGameObjectResourceManager->AddResource(pNewObject, objectName))
					{
						std::string msg("Adding resource : '" + std::string(objectName) + "' has failed.");
						throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
					}

					for (std::string line; getline(input, line);)
					{
						char componentType[256] = { 0 };
						if (1 == sscanf_s(line.c_str(), "[%s", componentType, 256))
						{
							// Serialize the game object component
							if (g_componentSerializationMap.find(componentType) != g_componentSerializationMap.end())
							{
								// Add component
								auto component = pNewObject->AddComponent(g_componentSerializationMap[componentType]);
								component->Serialize(input);
							}
							else
							{
								print_fstream(input);
								std::string msg(std::string(file_path) + " does not provide a valid component type for object type " + std::string(objectPath) + " at line : " + line);
								throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
							}
						}
						else
						{
							break;
						}
					}
				}
			}
			rewind_fstream(input);
			return 0;
		}
	}
}