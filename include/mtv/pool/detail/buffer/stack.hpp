#pragma once

#include "mtv/pool/detail/buffer/abstract.hpp"

#include <type_traits>
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
class TStack : public Traits::Abstract
{
    using Memory = typename Traits::Memory;
    using Type   = typename Traits::Type;
    
    static constexpr auto head = Traits::head;
    static constexpr auto tail = Traits::tail;
    
public:

    TStack() :
        _head{head(&_memory)},
        _tail{tail(&_memory)}
    {}
    
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
        auto temp = head(&_memory);
        
        while (temp != _head)
        {
            temp->~Type();
            temp++;
        }
        
        _head = head(&_memory);
    }
    
    ~TStack()
    {
        clean();    
    }
    
private:

          Type*       _head;
    const Type* const _tail;
    
    Memory _memory;
};

namespace stack
{

template <class StackType, std::size_t SIZE>
struct TTraits
{
    using Type = StackType;
    
    using Abstract = TAbstract<Type>;
    
    using Memory = std::aligned_storage_t<sizeof(Type) * SIZE, alignof(Type)>;
    
    static constexpr auto head = [](Memory* memoryPtr) { return reinterpret_cast<Type*>(memoryPtr)        ; };
    static constexpr auto tail = [](Memory* memoryPtr) { return reinterpret_cast<Type*>(memoryPtr) + SIZE ; }; 
};

template <class Type, std::size_t SIZE>
using TMake = TStack<TTraits<Type, SIZE>>;

} // namespace stack

} // namespace buffer

} // namespace detail

} // namespace pool

} // namespace mtv