#include "MemoryPool.h"
#include <iostream>
#include <string>

struct Chunk
{
    int a{1};
    int b{2};
    int c{3};
    int d{4};
    uint64_t arr[1000];

    int eval()
    {
        a = b;
        b = c;
        c = d;
        return a;
    }
};

// used for test

int main()
{

    memorypool::MemoryPool<Chunk, 100> mem_pool;
    mem_pool.init();
    timespec ts;
    ::clock_gettime(CLOCK_REALTIME, &ts);
    auto start = ts;
    Chunk* p[100];
    for (int i=0; i<100; i++)
    {
        p[i] = mem_pool.alloc();
        p[i]->eval();
        p[i]->a = i;
    }

    ::clock_gettime(CLOCK_REALTIME, &ts);
    auto mid = ts;

    Chunk* p2[100];
    for (int i=0; i<100; i++)
    {
        p2[i] = new Chunk();
        p2[i]->eval();
        p2[i]->a = i;
    }

    ::clock_gettime(CLOCK_REALTIME, &ts);
    auto end = ts;

    for (int i=0; i<100; i++)
    {
        mem_pool.dealloc(p[i]);
        delete p2[i];
    }

    std::cout << mid.tv_sec - start.tv_sec << "  " << mid.tv_nsec - start.tv_nsec << std::endl;
    std::cout << end.tv_sec - mid.tv_sec << "  " << end.tv_nsec - mid.tv_nsec << std::endl;
}