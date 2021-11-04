#pragma once
#include "MemoryChunk.h"
#include <exception>
#include <iostream>

namespace memorypool {

    template<unsigned int BLOCK_SIZE, unsigned int BLOCK_NUM>
    class MemoryPool
    {
        using Chunk = MemoryChunk<BLOCK_SIZE>;
    public:
        void init()
        {
            _freeList = (Chunk*) ::malloc(sizeof(Chunk) * BLOCK_NUM);
            Chunk* cur = _freeList;
            for (int i = 0; i < BLOCK_NUM - 1; i++)
            {
                // std::cout << "# " << i << " " << cur << std::endl;
                new (cur) Chunk();
                cur->next = cur + 1;
                cur = cur->next;
            }
        }

        template<typename T, typename ...Args>
        T* alloc(Args &&... args)
        {
            if constexpr (sizeof(T) > BLOCK_SIZE)
            {
                std::cerr << "Size exceeds\n";
                return nullptr;
            }

            if (_freeList)
            {
                auto res = _freeList->malloc(sizeof(T));
                // std::cout << _freeList->next << std::endl;
                _freeList = _freeList->next;
                new (res) T(std::forward<Args>(args)...);
                return reinterpret_cast<T*>(res);
            }
            throw std::bad_alloc();
        }

        void dealloc(void* p_)
        {
            if (!p_)
            {
                return;
            }

            auto cur = reinterpret_cast<Chunk*>(p_);
            new (p_) Chunk();
            cur->next = _freeList;
        }

    private:
        Chunk* _freeList{nullptr};
    };
}