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
class less
{

public:
    [[nodiscard]] constexpr bool operator()(const Type& a, const Type& b) const
    {
        return a < b;
    }
};

template<typename Type>
struct hash;

template<>
struct hash<unsigned>
{
    [[nodiscard]] constexpr unsigned operator()(unsigned value) const
    {
        // FNV-1a:
        unsigned basis = 0x811C9DC5;
        unsigned prime = 0x01000193;
        unsigned result = basis;
        result *= prime;
        result ^= value;
        return result;
    }
};

template<>
struct hash<bool>
{
    [[nodiscard]] constexpr unsigned operator()(bool value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

template<>
struct hash<char>
{
    [[nodiscard]] constexpr unsigned operator()(char value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

template<>
struct hash<int8_t>
{
    [[nodiscard]] constexpr unsigned operator()(int8_t value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

template<>
struct hash<uint8_t>
{
    [[nodiscard]] constexpr unsigned operator()(uint8_t value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

template<>
struct hash<int16_t>
{
    [[nodiscard]] constexpr unsigned operator()(int16_t value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

template<>
struct hash<uint16_t>
{
    [[nodiscard]] constexpr unsigned operator()(uint16_t value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

template<>
struct hash<int>
{
    [[nodiscard]] constexpr unsigned operator()(int value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

template<typename Type>
struct hash<Type*>
{
    [[nodiscard]] constexpr unsigned operator()(const Type* ptr) const
    {
        return hash<unsigned>()(unsigned(ptr));
    }
};

template<typename Type>
constexpr void hash_combine(const Type& value, unsigned& result)
{
    // FNV-1a:
    unsigned prime = 0x01000193;
    result *= prime;
    result ^= hash<Type>()(value);
}

[[nodiscard]] constexpr unsigned array_hash(const void* ptr, int size)
{
    // FNV-1a:
    unsigned basis = 0x811C9DC5;
    unsigned prime = 0x01000193;
    unsigned result = basis;

    if(ptr && size > 0)
    {
        auto u8_ptr = static_cast<const uint8_t*>(ptr);
        int index = 0;

        for(int limit = size / 4; index < limit; index += 4)
        {
            unsigned value = unsigned(u8_ptr[index]) |
                    unsigned(u8_ptr[index + 1]) << 8 |
                    unsigned(u8_ptr[index + 2]) << 16 |
                    unsigned(u8_ptr[index + 3]) << 24;
            result *= prime;
            result ^= value;
        }

        for(; index < size; ++index)
        {
            result *= prime;
            result ^= u8_ptr[index];
        }
    }

    return result;
}

template<>
struct hash<int64_t>
{
    [[nodiscard]] constexpr unsigned operator()(int64_t value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<>
struct hash<uint64_t>
{
    [[nodiscard]] constexpr unsigned operator()(uint64_t value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<>
struct hash<float>
{
    [[nodiscard]] constexpr unsigned operator()(float value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<>
struct hash<double>
{
    [[nodiscard]] constexpr unsigned operator()(double value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<>
struct hash<long double>
{
    [[nodiscard]] constexpr unsigned operator()(long double value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

template<typename Type>
[[nodiscard]] constexpr unsigned make_hash(const Type& value)
{
    return hash<Type>()(value);
}

}

#endif
