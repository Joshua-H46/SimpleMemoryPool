#pragma once
#include "MemoryChunk.h"
#include <exception>
#include <iostream>

namespace memorypool {

    template<typename T, unsigned int SIZE>
    class MemoryPool
    {
    public:
        void init()
        {
            MemoryChunk<T>* mem = (MemoryChunk<T>*) ::malloc(SIZE * sizeof(MemoryChunk<T>));
            freeList = mem;
            MemoryChunk<T>* cur = mem, *prev = nullptr;
            for (int i=0; i<SIZE - 1; i++)
            {
                cur->prev = prev;
                cur->next = (mem + i + 1);
                if (prev)
                    prev->next = cur;

                prev = cur;
                cur = cur->next;
            }
            cur->prev = prev;
            cur->next = nullptr;
            prev->next = cur;
        }

        T* alloc()
        {
            if (freeList)
            {
                auto res = freeList;
                freeList->prev = nullptr;
                freeList = freeList->next;
                if (freeList)
                {
                    freeList->prev = nullptr;
                }
                return res->alloc();
            }
            throw std::bad_alloc();
        }

        void dealloc(T* p_)
        {
            if (!p_)
            {
                return;
            }

            auto cur = (MemoryChunk<T>*) p_;
            if (cur->next)
            {
                cur->next->prev = cur->prev;
            }
            if (cur->prev)
            {
                cur->prev->next = cur->next;
            }
            cur->next = freeList;
            cur->prev = nullptr;
            if (freeList)
            {
                freeList->prev = cur;
            }
            else
            {
                freeList = cur;
            }
            
            if (!cur->dealloc())
            {
                std::cerr << "Failed to dealloc" << std::endl;
            }
        }

    private:
        // MemoryChunk<T>* mems;
        MemoryChunk<T>* freeList{nullptr};
        MemoryChunk<T>* usedList{nullptr};
    };
}