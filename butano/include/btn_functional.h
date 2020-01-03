#ifndef BTN_FUNCTIONAL_H
#define BTN_FUNCTIONAL_H

#include "btn_common.h"

namespace btn
{

template<typename Type>
class equal_to
{

public:
    [[nodiscard]] constexpr bool operator()(const Type& a, const Type& b) const
    {
        return a == b;
    }
};

template<typename Type>
struct hash;

template<>
struct hash<unsigned>
{
    [[nodiscard]] constexpr size_t operator()(unsigned value) const
    {
        // FNV-1a:
        size_t basis = 0x811C9DC5;
        size_t prime = 0x01000193;
        return (value ^ basis) * prime;
    }
};

template<>
struct hash<bool>
{
    [[nodiscard]] constexpr size_t operator()(bool value) const
    {
        return hash<size_t>()(size_t(value));
    }
};

template<>
struct hash<char>
{
    [[nodiscard]] constexpr size_t operator()(char value) const
    {
        return hash<size_t>()(size_t(value));
    }
};

template<>
struct hash<int8_t>
{
    [[nodiscard]] constexpr size_t operator()(int8_t value) const
    {
        return hash<size_t>()(size_t(value));
    }
};

template<>
struct hash<uint8_t>
{
    [[nodiscard]] constexpr size_t operator()(uint8_t value) const
    {
        return hash<size_t>()(size_t(value));
    }
};

template<>
struct hash<int16_t>
{
    [[nodiscard]] constexpr size_t operator()(int16_t value) const
    {
        return hash<size_t>()(size_t(value));
    }
};

template<>
struct hash<uint16_t>
{
    [[nodiscard]] constexpr size_t operator()(uint16_t value) const
    {
        return hash<size_t>()(size_t(value));
    }
};

template<>
struct hash<int>
{
    [[nodiscard]] constexpr size_t operator()(int value) const
    {
        return hash<size_t>()(size_t(value));
    }
};

template<typename Type>
struct hash<Type*>
{
    [[nodiscard]] constexpr size_t operator()(const Type* ptr) const
    {
        return hash<size_t>()(size_t(ptr));
    }
};

template<typename Type>
constexpr void hash_combine(const Type& value, size_t& result)
{
    // FNV-1a:
    size_t prime = 0x01000193;
    result = (hash<Type>()(value) ^ result) * prime;
}

[[nodiscard]] constexpr size_t array_hash(const void* ptr, size_t size)
{
    // FNV-1a:
    size_t basis = 0x811C9DC5;
    size_t result = basis;

    if(ptr)
    {
        auto u8_ptr = static_cast<const uint8_t*>(ptr);
        size_t index = 0;

        for(size_t limit = size / 4; index < limit; index += 4)
        {
            int value = u8_ptr[index] |
                    u8_ptr[index + 1] << 8 |
                    u8_ptr[index + 2] << 16 |
                    u8_ptr[index + 3] << 24;
            hash_combine(value, result);
        }

        for(; index < size; ++index)
        {
            hash_combine(u8_ptr[index], result);
        }
    }

    return result;
}

template<>
struct hash<int64_t>
{
    [[nodiscard]] constexpr size_t operator()(int64_t value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<>
struct hash<uint64_t>
{
    [[nodiscard]] constexpr size_t operator()(uint64_t value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<>
struct hash<float>
{
    [[nodiscard]] constexpr size_t operator()(float value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<>
struct hash<double>
{
    [[nodiscard]] constexpr size_t operator()(double value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<>
struct hash<long double>
{
    [[nodiscard]] constexpr size_t operator()(long double value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<typename Type>
[[nodiscard]] constexpr size_t make_hash(const Type& value)
{
    return hash<Type>()(value);
}

}

#endif
