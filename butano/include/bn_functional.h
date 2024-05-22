/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FUNCTIONAL_H
#define BN_FUNCTIONAL_H

/**
 * @file
 * Standard library functional aliases header file.
 *
 * @ingroup functional
 */

#include "bn_alignment.h"
#include "bn_type_traits.h"

namespace bn
{

/**
 * @brief Function object for performing comparisons using the equal operator.
 *
 * @ingroup functional
 */
template<typename Type>
class equal_to
{

public:
    /**
     * @brief Indicates if a is equal to b or not.
     */
    [[nodiscard]] constexpr bool operator()(const Type& a, const Type& b) const
    {
        return a == b;
    }
};

/**
 * @brief Function object for performing comparisons using the less than operator.
 *
 * @ingroup functional
 */
template<typename Type>
class less
{

public:
    /**
     * @brief Indicates if a is less than b or not.
     */
    [[nodiscard]] constexpr bool operator()(const Type& a, const Type& b) const
    {
        return a < b;
    }
};

/**
 * @brief Hash function object.
 *
 * @ingroup functional
 */
template<typename Type>
struct hash;

/**
 * @brief Hash support for unsigned.
 *
 * @ingroup functional
 */
template<>
struct hash<unsigned>
{
    /**
     * @brief Returns the hash of the given unsigned.
     */
    [[nodiscard]] constexpr unsigned operator()(unsigned value) const
    {
        // FNV-1a:
        constexpr unsigned basis = 0x811C9DC5;
        constexpr unsigned prime = 0x01000193;
        constexpr unsigned result = basis * prime;

        return result ^ value;
    }
};

/**
 * @brief Hash support for bool.
 *
 * @ingroup functional
 */
template<>
struct hash<bool>
{
    /**
     * @brief Returns the hash of the given bool.
     */
    [[nodiscard]] constexpr unsigned operator()(bool value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

/**
 * @brief Hash support for char.
 *
 * @ingroup functional
 */
template<>
struct hash<char>
{
    /**
     * @brief Returns the hash of the given char.
     */
    [[nodiscard]] constexpr unsigned operator()(char value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

/**
 * @brief Hash support for int8_t.
 *
 * @ingroup functional
 */
template<>
struct hash<int8_t>
{
    /**
     * @brief Returns the hash of the given int8_t.
     */
    [[nodiscard]] constexpr unsigned operator()(int8_t value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

/**
 * @brief Hash support for uint8_t.
 *
 * @ingroup functional
 */
template<>
struct hash<uint8_t>
{
    /**
     * @brief Returns the hash of the given uint8_t.
     */
    [[nodiscard]] constexpr unsigned operator()(uint8_t value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

/**
 * @brief Hash support for int16_t.
 *
 * @ingroup functional
 */
template<>
struct hash<int16_t>
{
    /**
     * @brief Returns the hash of the given int16_t.
     */
    [[nodiscard]] constexpr unsigned operator()(int16_t value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

/**
 * @brief Hash support for uint16_t.
 *
 * @ingroup functional
 */
template<>
struct hash<uint16_t>
{
    /**
     * @brief Returns the hash of the given uint16_t.
     */
    [[nodiscard]] constexpr unsigned operator()(uint16_t value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

/**
 * @brief Hash support for int.
 *
 * @ingroup functional
 */
template<>
struct hash<int>
{
    /**
     * @brief Returns the hash of the given int.
     */
    [[nodiscard]] constexpr unsigned operator()(int value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

/**
 * @brief Hash support for pointers.
 *
 * @ingroup functional
 */
template<typename Type>
struct hash<Type*>
{
    /**
     * @brief Returns the hash of the given pointer.
     */
    [[nodiscard]] constexpr unsigned operator()(const Type* ptr) const
    {
        return hash<unsigned>()(unsigned(ptr));
    }
};

/**
 * @brief Hash support for enums.
 *
 * @ingroup functional
 */
template<typename EnumType>
requires(is_enum_v<EnumType>)
struct hash<EnumType>
{
    /**
     * @brief Returns the hash of the given enum.
     */
    [[nodiscard]] constexpr unsigned operator()(EnumType value) const
    {
        return hash<unsigned>()(unsigned(value));
    }
};

/**
 * @brief Combines the hash of the given value with another hash.
 * @param value result hash is combined with the hash of this value.
 * @param result The hash of the given value is combined with this hash, and stored in it.
 *
 * @ingroup functional
 */
template<typename Type>
constexpr void hash_combine(const Type& value, unsigned& result)
{
    // FNV-1a:
    unsigned prime = 0x01000193;
    result *= prime;
    result ^= hash<Type>()(value);
}

/**
 * @brief Calculates the hash of the given plain data array.
 * @param ptr Start position of the plain data array.
 * @param size Size in bytes of the plain data array.
 *
 * @ingroup functional
 */
[[nodiscard]] constexpr unsigned array_hash(const void* ptr, int size)
{
    // FNV-1a:
    constexpr unsigned basis = 0x811C9DC5;
    constexpr unsigned prime = 0x01000193;

    unsigned result = basis;

    if(ptr && size > 0)
    {
        int word_size = size / 4;
        int words = word_size;

        if(aligned<4>(ptr))
        {
            auto u32_ptr = static_cast<const uint32_t*>(ptr);

            while(words)
            {
                result *= prime;
                result ^= *u32_ptr;
                ++u32_ptr;
                --words;
            }

            ptr = u32_ptr;
        }
        else
        {
            auto u8_ptr = static_cast<const uint8_t*>(ptr);

            while(words)
            {
                unsigned value = unsigned(u8_ptr[0]) |
                        unsigned(u8_ptr[1]) << 8 |
                        unsigned(u8_ptr[2]) << 16 |
                        unsigned(u8_ptr[3]) << 24;

                result *= prime;
                result ^= value;
                u8_ptr += 4;
                --words;
            }

            ptr = u8_ptr;
        }

        size -= word_size * 4;

        if(size)
        {
            auto u8_ptr = static_cast<const uint8_t*>(ptr);
            unsigned value = *u8_ptr;
            ++u8_ptr;
            --size;

            while(size)
            {
                value = (value << 8) + *u8_ptr;
                ++u8_ptr;
                --size;
            }

            result *= prime;
            result ^= value;
        }
    }

    return result;
}

/**
 * @brief Hash support for int64_t.
 *
 * @ingroup functional
 */
template<>
struct hash<int64_t>
{
    /**
     * @brief Returns the hash of the given int64_t.
     */
    [[nodiscard]] constexpr unsigned operator()(int64_t value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

/**
 * @brief Hash support for uint64_t.
 *
 * @ingroup functional
 */
template<>
struct hash<uint64_t>
{
    /**
     * @brief Returns the hash of the given uint64_t.
     */
    [[nodiscard]] constexpr unsigned operator()(uint64_t value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

/**
 * @brief Hash support for float.
 *
 * @ingroup functional
 */
template<>
struct hash<float>
{
    /**
     * @brief Returns the hash of the given float.
     */
    [[nodiscard]] constexpr unsigned operator()(float value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

/**
 * @brief Hash support for double.
 *
 * @ingroup functional
 */
template<>
struct hash<double>
{
    /**
     * @brief Returns the hash of the given double.
     */
    [[nodiscard]] constexpr unsigned operator()(double value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

/**
 * @brief Hash support for long double.
 *
 * @ingroup functional
 */
template<>
struct hash<long double>
{
    /**
     * @brief Returns the hash of the given long double.
     */
    [[nodiscard]] constexpr unsigned operator()(long double value) const
    {
        return array_hash(&value, sizeof(value));
    }
};

/**
 * @brief Returns the hash of the given value.
 *
 * @ingroup functional
 */
template<typename Type>
[[nodiscard]] constexpr unsigned make_hash(const Type& value)
{
    return hash<Type>()(value);
}

}

#endif
