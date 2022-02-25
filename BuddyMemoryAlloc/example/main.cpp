#include <Buddy.h>
#include <cassert>

int main()
{
    Buddy<64, 4> buddy;

    auto addr_64 = buddy.alloc(64);
    assert(addr_64 != nullptr);
    printf("addr: %p\n", addr_64);

    auto addr_1 = buddy.alloc(1);
    assert(addr_1 == nullptr);

    buddy.dealloc(addr_64);
    addr_1 = buddy.alloc(1);
    assert(addr_1 != nullptr);
    printf("addr: %p\n", addr_1);

    auto addr_32 = buddy.alloc(27);
    assert(addr_32 != nullptr);
    printf("addr: %p\n", addr_32);

    auto addr_16 = buddy.alloc(9);
    assert(addr_16 != nullptr);
    printf("addr: %p\n", addr_16);
    assert(buddy.alloc(9) == nullptr);
    buddy.dealloc(addr_1);
    auto addr_16_2 = buddy.alloc(9);
    assert(addr_16_2 != nullptr);
    printf("addr: %p\n", addr_16_2);
}