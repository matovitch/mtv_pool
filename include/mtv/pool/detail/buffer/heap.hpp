#pragma once

#include "mtv/pool/detail/buffer/abstract.hpp"

#include <cstdint>

namespace mtv
{

namespace pool
{

namespace detail
{

namespace buffer
{

template <class Traits>
class THeap : public Traits::Abstract
{
    using Type = typename Traits::Type;

public:

    THeap(const std::size_t size) :
        _memory{new Type[size]}
    {
        _head = _memory;
        _tail = _memory + size;
    }
    
    Type* allocate() override
    {
        if (_head == _tail)
        {
            return nullptr;    
        }
        
        return _head++;
    }
    
    void clean()
    {
        auto temp = _memory;
        
        while (temp != _head)
        {
            temp->~Type();
            temp++;
        }
        
        _head = _memory;
    }
    
    ~THeap()
    {
        clean();
        delete[] _memory;
        _memory = nullptr;
    }
    
private:

          Type* _head;
    const Type* _tail;
          Type* _memory;
};

namespace heap
{

template <class HeapType>
struct TTraits
{
    using Type     = HeapType;
    using Abstract = TAbstract<Type>;
};

template <class Type>
using TMake = THeap<TTraits<Type>>;

} // namespace heap

} // namespace buffer

} // namespace detail

} // namespace pool

} // namespace mtv