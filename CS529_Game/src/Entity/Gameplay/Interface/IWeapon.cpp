#include "IWeapon.hpp"
#include "src/SerializedLoader.hpp"

int My::IWeapon::max_level = MAX_WEAPON_LVL;

My::IWeapon::IWeapon(WEAPON_TYPE type, void* owner, std::string name) noexcept
	:
	IMyGameObject(MYGAMEOBJECT_TYPE::WEAPON, owner, name),
	m_type(type),
	m_level(1)
{
}

int My::IWeapon::Tick(double deltaTime)
{
	if (deltaTime != 0.0)
	{
		auto temp = GetComponent(COMPONENT_TYPE::SPRITE);
		if (temp)
		{
			auto p_spriteCom = static_cast<Component::Sprite*>(temp);
			switch (m_level)
			{
			case 1:
				p_spriteCom->SetCurrentSpriteByName("lvl1");
				break;
			case 2:
				p_spriteCom->SetCurrentSpriteByName("lvl2");
				break;
			case 3:
				p_spriteCom->SetCurrentSpriteByName("lvl3");
				break;
			default:
				break;
			}
		}
	}
	IMyGameObject::Tick(deltaTime);
	return 0;
}

int My::IWeapon::Draw()
{
	if (IsActive())
	{
		if (auto sceneCom = GetComponent(COMPONENT_TYPE::SCENECOMPONENT))
		{
			static_cast<Component::SceneComponent*>(sceneCom)->Draw();
		}
	}
	return 0;
}

void My::IWeapon::Serialize(std::wostream& wostream)
{
	wostream << str2wstr("Weapon : " + m_name + "\n");
	m_componentManager.Serialize(wostream);
	wostream << "\n";
	for (auto& child : m_children)
	{
		child->Serialize(wostream);
	}
}

My::WEAPON_TYPE My::IWeapon::GetType() const noexcept 
{
	return m_type;
}

void My::IWeapon::UpgradeWeapons()
{
	if (m_level < max_level) m_level++;
}

void My::IWeapon::DowngradeWeapons()
{
	if (m_level > 1) m_level--;
}

void My::IWeapon::SetUpgradeLevel(int lvl)
{
	m_level = lvl;
}

namespace My
{
	IWeapon* LoadIWeaponFromFile(void* owner,const char* file_path, std::string name)
	{
		if (g_gameObjectConfigurationIfstreamMap.find(Str(file_path)) == g_gameObjectConfigurationIfstreamMap.end())
		{
			std::string msg("The configuration file of game object " + Str(file_path) + " is not registered.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}

		IWeapon* pNewObject = nullptr;
		std::ifstream& input(g_gameObjectConfigurationIfstreamMap[Str(file_path)]);

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
				if (g_weaponSerializationMap.find(objectType) != g_weaponSerializationMap.end())
				{
					/* owner = g_pRootObject.get(), pool = g_pMemoryManager.get() */
					pNewObject = g_weaponCreatorMap[g_weaponSerializationMap[objectType]]->Create(owner, g_pMemoryManager.get(), name);
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
}