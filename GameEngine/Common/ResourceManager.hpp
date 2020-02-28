#pragma once
#include <map>
#include <string>

#include "Common/Interface/IRunTimeModule.hpp"
#include "Common/MemoryManager.hpp"

namespace My
{
	template<class T>
	class ResourceManager : public IRunTimeModule
	{
	public:
		ResourceManager() noexcept {};
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

		virtual int Initialize() override { return 0; };
		virtual int Tick(double deltaTime) override { return 0; };
		virtual int Finalize() override { return 0; };

		/* clear the map struct managed by the resource manager.*/
		virtual int Clear()
		{
			m_resourceMap.clear();
			return 0;
		}

		/* Use Memory Manager's Delete function to free all allocated objects (useful for game objects). */
		int FreeByNameWithMemoryManager(std::string name)
		{
			T* ret = GetResourceByName(name);
			if (!ret)
			{
				return 1;
			}
			g_pMemoryManager->Delete(ret);
			m_resourceMap.erase(name);
			return 0;
		}

		/* Remove the key value without freeing resources (useful for components). */
		int FreeByNameWithDefault(std::string name)
		{
			T* ret = GetResourceByName(name);
			if (!ret)
			{
				return 1;
			}
			m_resourceMap.erase(name);
			return 0;
		}

		/**
		 *	\brief AddResource: Add a resource pointer to the resource map. If the name already existed, no resouce is added.
		 *
		 *	std::string msg("Adding resource : '" + name + "' has failed.");
		 *	throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		 *  \author Hang Yu
		 *	\date 10/13/2019
		 *	\return 0 on success,1 on faliure.
		 *	\exception Should throw by the client on faliure.
		 *	@param name Name of the resource is a nullptr pointer.
		 *	@param resource Pointer to the resource.
		 */
		int AddResource(T* resource, std::string name)
		{
			if (resource)
			{
				if (m_resourceMap.find(name) != m_resourceMap.end())
				{
					return 1;
				}
				m_resourceMap[name] = resource;
				return 0;
			}
			return 1;
		}

		/**
		 *	\brief GetResource: return resource pointer by name.
		 *
		 *	Sample exception:
		 *	std::string msg("Getting resource : '" + name + "' has failed because it does not exist in the responsible resource manager.");
		 *	throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		 *  \author Hang Yu
		 *	\date 10/13/2019
		 *	\return nullptr on faliure.
		 *	\exception Should throw by the client on faliure.
		 *	@param name Name of the resource.
		 */
		T* GetResourceByName(std::string name)
		{
			if (m_resourceMap.find(name) != m_resourceMap.end())
			{
				return m_resourceMap[name];
			}
			return nullptr;
		}

		std::map<std::string, T*>& GetAllResources()
		{
			return m_resourceMap;
		}

	protected:
		std::map<std::string, T*> m_resourceMap;
	};

}