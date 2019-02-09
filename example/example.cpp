#include "mtv/pool/pool.hpp"

#include <iostream>

using IntPool = mtv::pool::TMake<int, 4>;

int main()
{
    IntPool intPool;

    const int& anInt = intPool.make(42);

    std::cout << "Created an int of value: " << anInt << '\n';

    return 0;
}

