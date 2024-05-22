/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SSTREAM_H
#define BN_SSTREAM_H

/**
 * @file
 * bn::ostringstream and bn::stringstream header file.
 *
 * @ingroup string
 */

#include "bn_fixed_fwd.h"
#include "bn_string_fwd.h"

namespace bn
{

class string_view;

/**
 * @brief Implements output operations on string based streams.
 *
 * It effectively stores an instance of istring_base and performs output operations to it.
 *
 * @ingroup string
 */
class ostringstream
{

public:
    /**
     * @brief Constructor.
     * @param string string in which to append to.
     */
    explicit ostringstream(istring_base& string);

    /**
     * @brief Returns a const reference to the managed string.
     */
    [[nodiscard]] const istring& str() const
    {
        return *_string;
    }

    /**
     * @brief Returns a reference to the managed string.
     */
    [[nodiscard]] istring& str()
    {
        return *_string;
    }

    /**
     * @brief Returns a const pointer to the managed string.
     */
    [[nodiscard]] const istring* rdbuf() const
    {
        return _string;
    }

    /**
     * @brief Returns a pointer to the managed string.
     */
    [[nodiscard]] istring* rdbuf()
    {
        return _string;
    }

    /**
     * @brief Sets the managed string.
     * @param sb Pointer to new string to manage.
     * @return Pointer to new string to manage.
     */
    istring* rdbuf(istring_base* sb);

    /**
     * @brief Sets the managed string.
     * @param sb Reference to new string to manage.
     */
    void set_rdbuf(istring_base& sb);

    /**
     * @brief Returns the decimal precision of fixed point operations.
     */
    [[nodiscard]] int precision() const
    {
        return _precision;
    }

    /**
     * @brief Sets the decimal precision of fixed point operations.
     * @param new_precision New decimal precision.
     * @return New decimal precision.
     */
    int precision(int new_precision);

    /**
     * @brief Sets the decimal precision of fixed point operations.
     * @param precision New decimal precision.
     */
    void set_precision(int precision);

    /**
     * @brief Returns a view over the contents of the managed string.
     */
    [[nodiscard]] string_view view() const;

    /**
     * @brief Returns the number of characters in the managed string.
     */
    [[nodiscard]] int size() const;

    /**
     * @brief Returns the number of characters in the managed string.
     */
    [[nodiscard]] int length() const;

    /**
     * @brief Returns the maximum possible number of characters in the managed string.
     */
    [[nodiscard]] int max_size() const;

    /**
     * @brief Returns the remaining characters capacity in the managed string.
     */
    [[nodiscard]] int available() const;

    /**
     * @brief Indicates if the managed string doesn't contain any character.
     */
    [[nodiscard]] bool empty() const;

    /**
     * @brief Indicates if the managed string can't contain any more characters.
     */
    [[nodiscard]] bool full() const;

    /**
     * @brief Appends an additional character to the managed string.
     * @param character Character to append.
     */
    void append(char character);

    /**
     * @brief Appends an additional character to the managed string.
     * @param character Character to append.
     */
    ostringstream& put(char character)
    {
        append(character);
        return *this;
    }

    /**
     * @brief Appends additional characters to the managed string.
     * @param view string_view to append.
     */
    void append(const string_view& view);

    /**
     * @brief Appends additional characters to the managed string.
     * @param string string to append.
     */
    void append(const istring_base& string);

    /**
     * @brief Appends additional characters to the managed string.
     * @param char_array_ptr Pointer to null-terminated characters array.
     */
    void append(const char* char_array_ptr);

    /**
     * @brief Appends additional characters to the managed string.
     * @param char_array_ptr Pointer to characters array.
     * @param char_array_size Characters count of the characters array.
     */
    void append(const char* char_array_ptr, int char_array_size);

    /**
     * @brief Appends additional characters to the managed string.
     * @param char_array_ptr Pointer to characters array.
     * @param char_array_size Characters count of the characters array.
     * @return Reference to this.
     */
    ostringstream& write(const char* char_array_ptr, int char_array_size)
    {
        append(char_array_ptr, char_array_size);
        return *this;
    }

    /**
     * @brief Appends additional characters to the managed string.
     * @param char_array Non empty const characters array.
     */
    template<int MaxSize>
    void append(char(&char_array)[MaxSize])
    {
        append(char_array, MaxSize);
    }

    /**
     * @brief Appends the character representation of the given bool value to the managed string.
     */
    void append(bool value)
    {
        append(value ? "true" : "false");
    }

    /**
     * @brief Appends the character representation of the given int value to the managed string.
     */
    void append(int value);

    /**
     * @brief Appends the character representation of the given long value to the managed string.
     */
    void append(long value);

    /**
     * @brief Appends the character representation of the given int64_t value to the managed string.
     */
    void append(int64_t value);

    /**
     * @brief Appends the character representation of the given unsigned value to the managed string.
     */
    void append(unsigned value);

    /**
     * @brief Appends the character representation of the given unsigned long value to the managed string.
     */
    void append(unsigned long value);

    /**
     * @brief Appends the character representation of the given uint64_t value to the managed string.
     */
    void append(uint64_t value);

    /**
     * @brief Appends the character representation of the given pointer to the managed string.
     */
    void append(const void* ptr);

    /**
     * @brief Appends the character representation of the given null pointer to the managed string.
     */
    void append([[maybe_unused]] const nullptr_t& null_ptr)
    {
        append("nullptr");
    }

    /**
     * @brief Appends the character representation of the given fixed point value to the managed string.
     */
    template<int Precision>
    void append(fixed_t<Precision> value)
    {
        if(value < 0)
        {
            append('-');
            append(0 - value);
        }
        else
        {
            int old_size = size();
            append(value.integer());

            int integer_digits = size() - old_size;
            int fraction_digits = precision() - integer_digits;

            if(fraction_digits > 0)
            {
                if(int fraction = value.fraction())
                {
                    unsigned zeros = 1;

                    for(int index = 0; index < fraction_digits; ++index)
                    {
                        zeros *= 10;
                    }

                    auto fraction_result = unsigned((uint64_t(fraction) * zeros) / value.scale());

                    if(fraction_result)
                    {
                        _append_fraction(fraction_result, fraction_digits);
                    }
                }
            }
        }
    }

    /**
     * @brief Appends the character representation of the given parameters to the managed string.
     */
    template<typename Type, typename... Args>
    void append_args(const Type& value, const Args&... args)
    {
        *this << value;
        append_args(args...);
    }

    /**
     * @brief Appends the character representation of the given value to the managed string.
     */
    template<typename Type>
    void append_args(const Type& value)
    {
        *this << value;
    }

    /// @cond DO_NOT_DOCUMENT

    static void append_args()
    {
    }

    /// @endcond

    /**
     * @brief Exchanges the contents of this ostringstream with those of the other one.
     * @param other ostringstream to exchange the contents with.
     */
    void swap(ostringstream& other);

    /**
     * @brief Exchanges the contents of a ostringstream with those of another one.
     * @param a First ostringstream to exchange the contents with.
     * @param b Second ostringstream to exchange the contents with.
     */
    friend void swap(ostringstream& a, ostringstream& b)
    {
        a.swap(b);
    }

private:
    istring* _string;
    int _precision = 6;

    void _append_fraction(unsigned fraction_result, int fraction_digits);
};


using stringstream = ostringstream; //!< stringstream alias.


/**
 * @brief Appends an additional character to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param character Character to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, char character)
{
    stream.append(character);
    return stream;
}

/**
 * @brief Appends additional characters to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param view string_view to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, const string_view& view)
{
    stream.append(view);
    return stream;
}

/**
 * @brief Appends additional characters to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param string string to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, const istring_base& string)
{
    stream.append(string);
    return stream;
}

/**
 * @brief Appends additional characters to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param char_array_ptr Pointer to null-terminated characters array.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, const char* char_array_ptr)
{
    stream.append(char_array_ptr);
    return stream;
}

/**
 * @brief Appends additional characters to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param char_array Non empty const characters array.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
template<int MaxSize>
inline ostringstream& operator<<(ostringstream& stream, char(&char_array)[MaxSize])
{
    stream.append<MaxSize>(char_array);
    return stream;
}

/**
 * @brief Appends the character representation of the given bool value to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param value bool value to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, bool value)
{
    stream.append(value);
    return stream;
}

/**
 * @brief Appends the character representation of the given int value to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param value int value to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, int value)
{
    stream.append(value);
    return stream;
}

/**
 * @brief Appends the character representation of the given long value to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param value long value to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, long value)
{
    stream.append(value);
    return stream;
}

/**
 * @brief Appends the character representation of the given int64_t value to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param value int64_t value to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, int64_t value)
{
    stream.append(value);
    return stream;
}

/**
 * @brief Appends the character representation of the given unsigned value to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param value unsigned value to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, unsigned value)
{
    stream.append(value);
    return stream;
}

/**
 * @brief Appends the character representation of the given unsigned long value to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param value unsigned long value to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, unsigned long value)
{
    stream.append(value);
    return stream;
}

/**
 * @brief Appends the character representation of the given uint64_t value to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param value uint64_t value to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, uint64_t value)
{
    stream.append(value);
    return stream;
}

/**
 * @brief Appends the character representation of the given pointer to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param ptr pointer to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, const void* ptr)
{
    stream.append(ptr);
    return stream;
}

/**
 * @brief Appends the character representation of the given null pointer to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param null_ptr Null pointer to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
inline ostringstream& operator<<(ostringstream& stream, const nullptr_t& null_ptr)
{
    stream.append(null_ptr);
    return stream;
}

/**
 * @brief Appends the character representation of the given fixed point value to the given ostringstream.
 * @param stream ostringstream in which to append to.
 * @param value Fixed point value to append.
 * @return Reference to the ostringstream.
 *
 * @ingroup string
 */
template<int Precision>
ostringstream& operator<<(ostringstream& stream, fixed_t<Precision> value)
{
    stream.append(value);
    return stream;
}

}

#endif
