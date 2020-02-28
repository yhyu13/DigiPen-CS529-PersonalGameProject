#include "MemoryPool.hpp"

const size_t My::MemoryPool::kPageSize = 1024;

My::MemoryPool::MemoryPool()
{
	m_PoolSize = 0;
	m_PoolMaxSize = kPageSize;
	m_pPoolHead = reinterpret_cast<uint8_t*>(malloc(m_PoolMaxSize));
	m_pPoolTail = m_pPoolHead;
	if (m_pPoolTail == nullptr)
	{
		std::string msg("Memory pool initialization has failed.");
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
	}
}

My::MemoryPool::~MemoryPool()
{
	Reset();
}

void* My::MemoryPool::Allocate(size_t size) 
{
	if ((m_PoolSize + size) > m_PoolMaxSize) { ResizePool(2); }
	void* ret = reinterpret_cast<void*>(m_pPoolTail);
	/* Update book keeping pointer */
	m_bookkeeper.push_back(ret);
	/* Update m_pPoolTail */
	m_pPoolTail += size;
	m_PoolSize += size;
	return ret;
}

void My::MemoryPool::Reset()
{
	if (m_pPoolHead) free(m_pPoolHead);
	m_pPoolHead = nullptr;
}

const std::vector<void*>& My::MemoryPool::GetBookKeeper()
{
	return m_bookkeeper;
}

void My::MemoryPool::ResizePool(size_t scalar)
{
	if (scalar > 1)
	{
		m_PoolMaxSize += kPageSize;
		uint8_t* temp = reinterpret_cast<uint8_t*>(realloc(m_pPoolHead, m_PoolMaxSize));
		if (!temp)
		{
			Reset();
			std::string msg("Realloc has failed.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}
		/* Update book keeping pointers */
		size_t shift = temp - m_pPoolHead;
		for (auto& ptr : m_bookkeeper)
		{
			ptr = reinterpret_cast<uint8_t*>(ptr) + shift;
		}
		/* Update m_pPoolHead && m_pPoolTail */
		size_t diff = m_pPoolTail - m_pPoolHead;
		m_pPoolHead = temp;
		m_pPoolTail = m_pPoolHead + diff;
	}
}
