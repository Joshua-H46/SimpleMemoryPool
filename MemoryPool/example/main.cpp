#include "MemoryPool.h"
#include <iostream>
#include <string>

struct Chunk
{
    Chunk(int a, int b, int c) : a(a), b(b), c(c) {}
    int a{1};
    int b{2};
    int c{3};
    char arr[1012];

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
    timespec ts;
    ::clock_gettime(CLOCK_REALTIME, &ts);
    auto start = ts;
    Chunk* p[1000];
    for (int i=0; i<1000; i++)
    {
        // std::cout << "# " << i << std::endl;
        p[i] = mem_pool.alloc<Chunk>(i, i+1, i+2);
        p[i]->eval();
        // p[i]->a = i;
    }

    ::clock_gettime(CLOCK_REALTIME, &ts);
    auto mid = ts;

    Chunk* p2[1000];
    for (int i=0; i<1000; i++)
    {
        p2[i] = new Chunk(i, i+1, i+2);
        p2[i]->eval();
        // p2[i]->a = i;
    }

    ::clock_gettime(CLOCK_REALTIME, &ts);
    auto end = ts;

    for (int i=0; i<1000; i++)
    {
        // std::cout << p[i]->a << std::endl;
        mem_pool.dealloc(p[i]);
        delete p2[i];
    }

    std::cout << mid.tv_sec - start.tv_sec << "  " << mid.tv_nsec - start.tv_nsec << std::endl;
    std::cout << end.tv_sec - mid.tv_sec << "  " << end.tv_nsec - mid.tv_nsec << std::endl;
}