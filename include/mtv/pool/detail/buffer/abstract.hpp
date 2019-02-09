#pragma once

namespace mtv
{

namespace pool
{

namespace detail
{

namespace buffer
{

template <class Type>
struct TAbstract
{
    virtual Type* allocate() = 0;

    virtual ~TAbstract() {}  
};

} // namespace buffer

} // namespace detail

} // namespace pool

} // namespace mtv