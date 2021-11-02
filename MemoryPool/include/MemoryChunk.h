#pragma once

namespace memorypool {

    // One memory chunk holds a object of T (or a bunch of memory)
    template<typename T>
    class MemoryChunk
    {
    public:
        T* alloc()
        {
            if (isUsed())
            {
                return nullptr;
            }
            
            _used = true;
            return &_t;
        }

        bool dealloc()
        {
            if (!isUsed())
            {
                return false;
            }
            _used = false;
            return true;
        }

        bool isUsed() const
        {
            return _used;
        }

    // private:
        T _t;
        MemoryChunk<T>* next{nullptr};
        MemoryChunk<T>* prev{nullptr};
        bool _used{false};
    };
}