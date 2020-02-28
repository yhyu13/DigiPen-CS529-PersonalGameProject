#include "WeaponarySystem.hpp"
#include "Common/Utility/Utility.hpp"
#include "Common/Entity/GameObjectResourceManager.hpp"

My::Component::WeaponarySystem::WeaponarySystem(void* owner, std::string name)
	:
	IComponent((COMPONENT_TYPE::WEAPONARYSYSTEM), owner, name),
	m_maxWeapons(0)
{
}

void My::Component::WeaponarySystem::Update(double deltaTime)
{
}

 My::Component::WeaponarySystem::~WeaponarySystem()
{
	for (auto& map : m_weaponMaps)
	{
		for (auto it = map.begin(); it != map.end(); ++it)
		{
			it->second->SetGCToTrue();
		}
	}
}

void My::Component::WeaponarySystem::Serialize(std::ifstream& fstream)
{
	for (std::string line; getline(fstream, line);)
	{
		int id = 0;
		char weaponName[256] = { 0 };
		char weaponType[256] = { 0 };
		if (1 == sscanf_s(line.c_str(), "Max %d", &m_maxWeapons))
		{
			for (int i = 0; i < m_maxWeapons; ++i)
			{
				m_currentWeapons.push_back(nullptr);
				m_weaponMaps.push_back(std::map<std::string, IWeapon*>());
			}
		}
		else if (3 == sscanf_s(line.c_str(), "%d %s %s", &id, &weaponType, 256,&weaponName, 256))
		{
			if (id-- > m_maxWeapons)
			{
				print_fstream(fstream);
				std::string msg("File does not provide a valid weapon id (max :" + Str(m_maxWeapons) + ") " + std::string(weaponType) + " at line : " + line);
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
			}

			// Serialize the game object and set its owner to be the root object.
			if (g_weaponSerializationMap.find(weaponType) != g_weaponSerializationMap.end())
			{
				auto pNewObject = LoadIWeaponFromFile(this, weaponType, weaponName);
				m_weaponMaps[id][weaponName] = pNewObject;
				if (m_currentWeapons[id]) m_currentWeapons[id]->SetActive(false);
				m_currentWeapons[id] = pNewObject;
				m_currentWeapons[id]->SetActive(true);

				// Add game object under the resource manager.
				if (1 == g_pGameObjectResourceManager->AddResource(pNewObject, weaponName))
				{
					std::string msg("Adding resource : '" + std::string(weaponName) + "' has failed.");
					throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
				}
			}
			else
			{
				print_fstream(fstream);
				std::string msg("File does not provide a valid game object of type: " + std::string(weaponName) + " at line : " + line);
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
			}
		}
		else if (line[0] == ']')
		{
			return;
		}
		else
		{
			print_fstream(fstream);
			std::string msg("WeaponSystem serialization has failed at line " + line + ".");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
	}
}

void My::Component::WeaponarySystem::Serialize(std::wostream& wostream)
{
	wostream << "[Component : WeaponSystem ";
	for (int i = 0; i < m_maxWeapons; ++i)
	{
		for (auto it = m_weaponMaps[i].begin(); it != m_weaponMaps[i].end(); ++it)
		{
			wostream << str2wstr("Weapon : " + Str(i) + " " + it->first + "\n");
		}
	}
	wostream << "] ";
}

void My::Component::WeaponarySystem::UpgradeWeapons()
{
	for (auto& map : m_weaponMaps)
	{
		for (auto it = map.begin(); it != map.end(); ++it)
		{
			it->second->UpgradeWeapons();
		}
	}
}

void My::Component::WeaponarySystem::DowngradeWeapons()
{
	for (auto& map : m_weaponMaps)
	{
		for (auto it = map.begin(); it != map.end(); ++it)
		{
			it->second->DowngradeWeapons();
		}
	}
}

void My::Component::WeaponarySystem::ChangeWeapon(int id, std::string name)
{
	if (id < 1 && id > m_maxWeapons)
	{
		std::string msg("Trying to change weapon out of range: " + Str(id) + "/" + Str(m_maxWeapons));
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
	}
	else
	{
		id--;
		for (auto it = m_weaponMaps[id].begin(); it != m_weaponMaps[id].end(); ++it)
		{
			if ((*it).first.find(name) != std::string::npos)
			{
				if (m_currentWeapons[id]) m_currentWeapons[id]->SetActive(false);
				m_currentWeapons[id] = (*it).second;
				m_currentWeapons[id]->SetActive(true);
				return;
			}
		}
		std::string msg("Can not find weapon " + name + " in " + Str(id));
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
	}
}
