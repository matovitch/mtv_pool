#pragma once

#include "mtv/pool/detail/buffer/abstract.hpp"
#include "mtv/pool/detail/buffer/stack.hpp"
#include "mtv/pool/detail/buffer/heap.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace mtv
{

namespace pool
{

namespace detail
{

template <class Traits>
class TAllocator
{
    using Type = typename Traits::Type;
    
    using AbstractBuffer = typename Traits::AbstractBuffer; 
    using    StackBuffer = typename Traits::   StackBuffer;
    using     HeapBuffer = typename Traits::    HeapBuffer;
    
    static constexpr std::size_t SIZE = Traits::SIZE;
    
public:

    TAllocator() :
        _bufferPtr{&_stackBuffer},
        _size{SIZE}
    {}
    
    Type* allocate()
    {
        if (!_recycleds.empty())
        {
            Type* const ptr = _recycleds.back();
            _recycleds.pop_back();
            ptr->~Type();
            return ptr;
        }
        
        Type* const ptr = _bufferPtr->allocate();
        
        if (!ptr)
        {
            _heapBuffers.emplace_back(std::make_unique<HeapBuffer>(_size <<= 1));
            _bufferPtr = _heapBuffers.back().get();
        
            return _heapBuffers.back()->allocate();
        }
        
        return ptr;
    }
    
    void recycle(Type* ptr)
    {
        _recycleds.emplace_back(ptr);    
    }
    
private:

    AbstractBuffer*                          _bufferPtr;
    StackBuffer                              _stackBuffer;
    std::vector<Type*>                       _recycleds;
    std::vector<std::unique_ptr<HeapBuffer>> _heapBuffers;
    
    std::size_t _size;
};

namespace allocator
{

template <class TraitsType, std::size_t TRAITS_SIZE>
struct TTraits
{    
    static constexpr std::size_t SIZE = TRAITS_SIZE;

    using Type = TraitsType;
    
    using    StackBuffer = buffer::stack ::TMake <Type, SIZE>;
    using     HeapBuffer = buffer::heap  ::TMake <Type>;
    using AbstractBuffer = buffer::TAbstract     <Type>;
};

template <class Type, std::size_t SIZE>
using TMake = TAllocator<TTraits<Type, SIZE>>;

} // namespace allocator

} // namespace detail

} // namespace pool

} // namespace mtv
