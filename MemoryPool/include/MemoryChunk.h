#pragma once
#include <memory>
#include <cstddef>
#include <iostream>

namespace memorypool {

    // One memory chunk holds a object of T (or a bunch of memory)
    template<unsigned int SIZE>
    struct MemoryChunk
    {
        void* malloc(unsigned int size_)
        {
            return _mem;
        }

        void free() {}

        union
        {
            std::byte _mem[SIZE];
            MemoryChunk<SIZE>* next{nullptr};  
        };
    };
}