#include "MemoryPool.h"
#include <iostream>
#include <string>
#include <chrono>

struct Chunk
{
    Chunk(int a, int b, int c) : a(a), b(b), c(c) {}
    int a{1};
    int b{2};
    int c{3};
    char arr[500];

    int eval()
    {
        a = c;
        b = c;
        return a;
    }
};

// used for test

int main()
{

    memorypool::MemoryPool<1024, 1000> mem_pool;
    mem_pool.init();
    auto start = std::chrono::system_clock::now();
    Chunk* p[1000];
    for (int i=0; i<1000; i++)
    {
        // std::cout << "# " << i << std::endl;
        p[i] = mem_pool.alloc<Chunk>(i, i+1, i+2);
        p[i]->eval();
        // p[i]->a = i;
    }

    auto mid = std::chrono::system_clock::now();

    Chunk* p2[1000];
    for (int i=0; i<1000; i++)
    {
        p2[i] = new Chunk(i, i+1, i+2);
        p2[i]->eval();
        // p2[i]->a = i;
    }

    auto end = std::chrono::system_clock::now();

    for (int i=0; i<1000; i++)
    {
        // std::cout << p[i]->a << std::endl;
        mem_pool.dealloc(p[i]);
        delete p2[i];
    }

    std::cout << "The duration for creating objects with memory pool is: " << std::chrono::duration_cast<std::chrono::microseconds>(mid - start).count() << " microseconds" << std::endl;
    std::cout << "The duration for creating objects with new is: " << std::chrono::duration_cast<std::chrono::microseconds>(end - mid).count() << " microseconds" << std::endl;
}