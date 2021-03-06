#include "MemoryManager.hpp"

#ifndef ALIGN
#define ALIGN(x, a)         (((x) + ((a) - 1)) & ~((a) - 1))
#endif

using namespace My;

namespace My {

    const uint32_t MemoryManager::kBlockSizes[] = {
        // 4-increments
        4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
        52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 

        // 32-increments
        128, 160, 192, 224, 256, 288, 320, 352, 384, 
        416, 448, 480, 512, 544, 576, 608, 640, 

        // 64-increments
        704, 768, 832, 896, 960, 1024, 
		
		// Large
		2048, 4096
    };

    const uint32_t MemoryManager::kPageSize  = 8192;
    const uint32_t MemoryManager::kAlignment = 4;

    // number of elements in the block size array
    const uint32_t MemoryManager::kNumBlockSizes = 
        sizeof(kBlockSizes) / sizeof(kBlockSizes[0]);

    // largest valid block size (must not be greater than 2^16)
    const uint32_t MemoryManager::kMaxBlockSize = 
        kBlockSizes[kNumBlockSizes - 1];

    size_t*        MemoryManager::m_pBlockSizeLookup;
    Allocator*     MemoryManager::m_pAllocators;
}

int My::MemoryManager::Initialize() 
{
    // one-time initialization
    static bool s_bInitialized = false;
    if (!s_bInitialized) {
        // initialize block size lookup table
        m_pBlockSizeLookup = new size_t[kMaxBlockSize + 1];
        size_t j = 0;
        for (size_t i = 0; i <= kMaxBlockSize; i++) {
            if (i > kBlockSizes[j]) ++j;
            m_pBlockSizeLookup[i] = j;
        }

        // Throwing exception in noexcept would terminate the program, 
        // But since it's an exception in the memory allocator, it's find to kill the program anyway.
		if (j != kNumBlockSizes - 1)
		{
			std::string msg("Memory manger pool initialization has failed.");
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, My::str2wstr(msg));
		}

        // initialize the allocators
        m_pAllocators = new Allocator[kNumBlockSizes];
        for (size_t i = 0; i < kNumBlockSizes; i++) {
            m_pAllocators[i].Reset(kBlockSizes[i], kPageSize, kAlignment);
        }
        s_bInitialized = true;
    }

    return 0;
}

int My::MemoryManager::Finalize() noexcept
{
    delete[] m_pAllocators;
    delete[] m_pBlockSizeLookup;
	return 0;
}

int My::MemoryManager::Tick(double deltaTime) noexcept
{
	return 0;
}

Allocator* My::MemoryManager::LookUpAllocator(size_t size) noexcept
{
    // check eligibility for lookup
    return (size <= kMaxBlockSize) ? m_pAllocators + m_pBlockSizeLookup[size] : nullptr;
}

void* My::MemoryManager::Allocate(size_t size, size_t alignment) noexcept
{
    uint8_t* p;
    size += alignment;
    if (Allocator* pAlloc = LookUpAllocator(size))
        p = reinterpret_cast<uint8_t*>(pAlloc->Allocate());
    else
        p = reinterpret_cast<uint8_t*>(malloc(size));

    p = reinterpret_cast<uint8_t*>(ALIGN(reinterpret_cast<size_t>(p), alignment));
    
    return reinterpret_cast<void*>(p);
}

// Replacement for malloc()

void* My::MemoryManager::Allocate(size_t size) noexcept
{
	/*return malloc(size);*/
#if CUSTOM_ALLOCATOR 
	return (size <= kMaxBlockSize) ? (m_pAllocators + m_pBlockSizeLookup[size])->Allocate()
		: malloc(size);
#else
	return malloc(size);
#endif // CUSTOM_ALLOCATOR 
}

// Replacement for free()

void My::MemoryManager::Free(void* p, size_t size) noexcept
{
	// Original
	//   (size <= kMaxBlockSize) ? (m_pAllocators + m_pBlockSizeLookup[size])->Free(p) 
	//       : free(p);
#if CUSTOM_ALLOCATOR 
	// Storing m_szDataSize as uint16_t right before each block
	(size <= kMaxBlockSize) ? (m_pAllocators + m_pBlockSizeLookup[*(reinterpret_cast<uint16_t*>(p) - 1)])->Free(p)
		: free(p);
#else
	free(p);
#endif // CUSTOM_ALLOCATOR 
}

