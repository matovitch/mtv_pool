#pragma once

#include "mtv/pool/detail/allocator.hpp"

#include <cstdint>
#include <utility>

namespace mtv
{

template <class Traits>
class TPool
{
    using Type      = typename Traits::Type;
    using Allocator = typename Traits::Allocator;

public:

    template <class... Args>
    Type& make(Args&&... args)
    {
        return *(new(static_cast<void *>(_allocator.allocate())) Type(std::forward<Args>(args)...));
    }

    void recycle(Type& value)
    {
        _allocator.recycle(&value);    
    }

private:

    Allocator _allocator;
};

namespace pool
{

template <class TraitsType, std::size_t SIZE>
struct TTraits
{
    using Type = TraitsType;
    
    using Allocator = detail::allocator::TMake<Type, SIZE>;
};

template <class Type, std::size_t SIZE>
using TMake = TPool<TTraits<Type, SIZE>>;

} // namespace pool

} // namespace mtv
