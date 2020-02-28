#pragma once
#include <new>
#include <vector>
#include "Common/Utility/EngineException.hpp"

namespace My
{
	class MemoryPool
	{
	public:
		// Replacement for new 
		template<class T, typename... Arguments>
		T* New(Arguments... parameters)
		{
			return new (Allocate(sizeof(T))) T(parameters...);
		}
	public:
		MemoryPool();
		virtual ~MemoryPool();

		void* Allocate(size_t size);
		void Reset();
		const std::vector<void*>& GetBookKeeper();
	private:
		void ResizePool(size_t scalar);

	private:
		uint8_t* m_pPoolHead;
		uint8_t* m_pPoolTail;
		size_t m_PoolSize;
		size_t m_PoolMaxSize;
		static const size_t kPageSize;
		/* BookKeeps all the pointer to objects in the memory pool */
		std::vector<void*> m_bookkeeper;
	};
}