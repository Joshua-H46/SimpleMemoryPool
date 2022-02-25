#pragma once
#include <stdlib.h>
#include <iostream>

#define KB_TO_BYTES(size) size * 1024
#define IS_POWER_OF_TWO(x) (!((x) & (x-1)))
#define LEFT_CHILD(x) 2*x+1
#define RIGHT_CHILD(x) 2*x+2
#define PARENT(x) (x+1)/2-1

template<unsigned SIZE, unsigned PGSIZE>
class Buddy
{
    static_assert(IS_POWER_OF_TWO(SIZE));
    static_assert(IS_POWER_OF_TWO(PGSIZE));

    static constexpr unsigned NNODE = (SIZE / PGSIZE) * 2 - 1;
    struct ControlBlock
    {
        // size_t _size;
        size_t _size[NNODE];
    };

public:
    Buddy()
    {
        _mem = malloc(KB_TO_BYTES(SIZE));
        // set control block size
        int node_size = SIZE * 2;
        for (size_t i=0; i<NNODE; i++)
        {
            if (IS_POWER_OF_TWO(i+1))
            {
                node_size /= 2;
            }
            _cb._size[i] = node_size;
        }
    }

    ~Buddy()
    {
        free(_mem);
        _mem = nullptr;
    }

    void* alloc(unsigned size_)
    {
        if (size_ > _cb._size[0])
        {
            std::cerr << "No enough memory\n";
            return nullptr;
        }

        size_ = roundUpTo2sPower(size_);
        unsigned node = 0;
        unsigned node_size = SIZE;
        for (; node_size > size_; node_size = node_size >> 1)
        {
            if (_cb._size[LEFT_CHILD(node)] >= size_)
            {
                node = LEFT_CHILD(node);
            }
            else
            {
                node = RIGHT_CHILD(node);
            }
        }

        auto offset = node_size * (node + 1) - SIZE;        // offset in kb
        _cb._size[node] -= size_;
        while (node > 0)
        {
            node = PARENT(node);
            _cb._size[node] = std::max(_cb._size[LEFT_CHILD(node)], _cb._size[RIGHT_CHILD(node)]);
        }

        return _mem + KB_TO_BYTES(offset);
    }

    void dealloc(void* page_)
    {
        // first calculate the node from the page
        // Note that only the size of selected node is set to zero. The child nodes of the selected one are not changed
        auto index = ((unsigned long)page_ - (unsigned long)_mem)/1024/PGSIZE + SIZE/PGSIZE - 1;
        unsigned int node_size = PGSIZE;
        while (_cb._size[index] != 0)
        {
            index = PARENT(index);
            node_size = node_size << 1;
        }

        _cb._size[index] = node_size;
        while (index > 0)
        {
            index = PARENT(index);
            node_size *= 2;
            if (node_size == _cb._size[LEFT_CHILD(index)] + _cb._size[RIGHT_CHILD(index)])
            {
                _cb._size[index] = node_size;
            }
            else
            {
                _cb._size[index] = std::max(_cb._size[LEFT_CHILD(index)], _cb._size[RIGHT_CHILD(index)]);
            }
        }
        page_ = nullptr;
    }

private:
    unsigned roundUpTo2sPower(unsigned size_)
    {
        if (IS_POWER_OF_TWO(size_))
        {
            return std::max(size_, PGSIZE);
        }

        size_ |= size_ >> 1;
        size_ |= size_ >> 2;
        size_ |= size_ >> 4;
        size_ |= size_ >> 8;
        size_ |= size_ >> 16;
        return std::max(size_ + 1, PGSIZE);
    }


    void* _mem;
    ControlBlock _cb;
};