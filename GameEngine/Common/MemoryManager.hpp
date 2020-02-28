#pragma once
#include <memory>
#include "Common/Interface/IRunTimeModule.hpp"
#include "Common/Allocator.hpp"
#include "Common/Utility/EngineException.hpp"

#define CUSTOM_ALLOCATOR 0

namespace My {
    class MemoryManager : public IRunTimeModule
    {
    public:
        // Replacement for new 
        template<class T, typename... Arguments>
        T* New(Arguments... parameters) noexcept
        {
#if CUSTOM_ALLOCATOR 
			return new (Allocate(sizeof(T))) T(parameters...);
#else
			return new T(parameters...);
#endif // CUSTOM_ALLOCATOR    
        }

        // Replacement for delete
        template<class T>
        void Delete(T* p) noexcept
        {
#if CUSTOM_ALLOCATOR 
			p->~T();
			Free(p, sizeof(T));
#else
			delete p;
#endif // CUSTOM_ALLOCATOR 
           
        }

    public:
        virtual ~MemoryManager() noexcept {};

        virtual int Initialize() override;
        virtual int Finalize() noexcept override;
        virtual int Tick(double deltaTime) noexcept override;

        void* Allocate(size_t size, size_t alignment) noexcept;

        // Replacement for malloc()
		void* Allocate(size_t size) noexcept;
		
		// Replacement for free()
		void  Free(void* p, size_t size) noexcept;
		
    private:
        static const uint32_t kBlockSizes[];

        static const uint32_t kPageSize;
        static const uint32_t kAlignment;

        // number of elements in the block size array
        static const uint32_t kNumBlockSizes;

        // largest valid block size
        static const uint32_t kMaxBlockSize;

        static size_t*        m_pBlockSizeLookup;
        static Allocator*     m_pAllocators;
    private:
        static Allocator* LookUpAllocator(size_t size) noexcept;
    };

	extern std::shared_ptr<MemoryManager> g_pMemoryManager;
}

namespace My {

    #ifndef MyVector
    #define MyVector(T) std::vector<T, My::allocator<T>>
    #endif

    template <typename T>
    class allocator
    {
    public:
        
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef const T *const_pointer;
        typedef const T &const_reference;
        typedef unsigned size_type;
        typedef unsigned difference_type;
        
        template <typename U> struct rebind { typedef std::allocator<U> other;};
        
        pointer allocate(unsigned n)
        {
        return
            reinterpret_cast<T *>
            (g_pMemoryManager->Allocate(sizeof(T) * n));
        }

        void deallocate(pointer p, unsigned n)
        {
        g_pMemoryManager->Free(p, sizeof(T) * n);
        }

        void construct(pointer p, const_reference clone)
        {
        new (p) T(clone);
        }

        void destroy(pointer p)
        {
        p->~T();
        }

        pointer address(reference x) const
        {
        return &x;
        }

        const_pointer address(const_reference x) const
        {
        return &x;
        }

        bool operator==(const allocator &rhs)
        {
            return true;
        }

        bool operator!=(const allocator &rhs)
        {
        return !operator==(rhs);
        }
    };
}