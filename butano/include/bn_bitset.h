/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 jwellbelove, https://github.com/ETLCPP/etl, http://www.etlcpp.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef BN_BITSET_H
#define BN_BITSET_H

/**
 * @file
 * bn::bitset implementation header file.
 *
 * @ingroup bitset
 */

#include "bn_bit.h"
#include "bn_span.h"
#include "bn_limits.h"
#include "bn_bitset_fwd.h"

namespace bn
{

/**
 * @brief Base class of bn::bitset.
 *
 * Can be used as a reference type for all bn::bitset containers.
 *
 * @ingroup bitset
 */
class ibitset
{

public:
    using element_t = uint8_t; //!< Element type alias.

    /**
     * @brief Proxy to allow to interact with individual bits of a ibitset.
     */
    class reference
    {

    public:
        /**
         * @brief Returns the value of the referenced bit.
         */
        [[nodiscard]] constexpr operator bool() const
        {
            return _bitset->test(_index);
        }

        /**
         * @brief Returns the inverse of the referenced bit.
         */
        [[nodiscard]] constexpr bool operator~() const
        {
            return !_bitset->test(_index);
        }

        /**
         * @brief Assigns a value to the referenced bit.
         * @param value Value to assign.
         * @return Reference to this.
         */
        constexpr reference& operator=(bool value)
        {
            _bitset->set(_index, value);
            return *this;
        }

        /**
         * @brief Assigns a value to the referenced bit.
         * @param other Reference to the value to assign.
         * @return Reference to this.
         */
        constexpr reference& operator=(const reference& other)
        {
            _bitset->set(_index, bool(other));
            return *this;
        }

        /**
         * @brief Flips the referenced bit.
         * @return Reference to this.
         */
        constexpr reference& flip()
        {
            _bitset->flip(_index);
            return *this;
        }

    private:
        friend class ibitset;

        ibitset* _bitset;
        int _index;

        constexpr reference(ibitset& bitset, int position) :
            _bitset(&bitset),
            _index(position)
        {
        }
    };

    ibitset(const ibitset& other) = delete;

    /**
     * @brief Copy assignment operator.
     * @param other ibitset to copy.
     * @return Reference to this.
     */
    constexpr ibitset& operator=(const ibitset& other)
    {
        if(this != &other)
        {
            int num_elements = _num_elements;
            BN_BASIC_ASSERT(num_elements == other._num_elements, "Different size: ", size(), " - ", other.size());

            copy_n(other._data, num_elements, _data);
        }

        return *this;
    }

    /**
     * @brief Returns a const pointer to the beginning of the internal data.
     */
    [[nodiscard]] constexpr const element_t* data() const
    {
        return _data;
    }

    /**
     * @brief Returns the bits count.
     */
    [[nodiscard]] constexpr int size() const
    {
        return _num_elements * _bits_per_element;
    }

    /**
     * @brief Returns the elements count.
     */
    [[nodiscard]] constexpr int elements_size() const
    {
        return _num_elements;
    }

    /**
     * @brief Returns the number of bits that are set to `true`.
     */
    [[nodiscard]] constexpr int count() const
    {
        int result = 0;

        for(int index = _num_elements - 1; index >= 0; --index)
        {
            result += popcount(_data[index]);
        }

        return result;
    }

    /**
     * @brief Returns the value stored at the specified index.
     */
    [[nodiscard]] constexpr bool test(int index) const
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index, " - ", size());

        return (_element(index) & _bit(index)) != 0;
    }

    /**
     * @brief Sets all bits to `true`.
     * @return Reference to this.
     */
    constexpr ibitset& set()
    {
        fill_n(_data, _num_elements, _all_bits_set);
        return *this;
    }

    /**
     * @brief Sets the specified bit to `true`.
     * @param index Index of the bit to set.
     * @return Reference to this.
     */
    constexpr ibitset& set(int index)
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index, " - ", size());

        _element(index) |= _bit(index);
        return *this;
    }

    /**
     * @brief Sets the specified bit to the specified value.
     * @param index Index of the bit to set.
     * @param value Value to set.
     * @return Reference to this.
     */
    constexpr ibitset& set(int index, bool value)
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index, " - ", size());

        element_t& element = _element(index);
        element_t bit = _bit(index);

        if(value)
        {
            element |= bit;
        }
        else
        {
            element &= ~bit;
        }

        return *this;
    }

    /**
     * @brief Sets all bits to `false`.
     * @return Reference to this.
     */
    constexpr ibitset& reset()
    {
        fill_n(_data, _num_elements, _all_bits_clear);
        return *this;
    }

    /**
     * @brief Sets the specified bit to `false`.
     * @param index Index of the bit to set.
     * @return Reference to this.
     */
    constexpr ibitset& reset(int index)
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index, " - ", size());

        _element(index) &= ~_bit(index);
        return *this;
    }

    /**
     * @brief Flips all values (changes `true` values to `false` and vice versa).
     * @return Reference to this.
     */
    constexpr ibitset& flip()
    {
        int last_element_index = _num_elements - 1;

        for(int index = last_element_index; index >= 0; --index)
        {
            _data[index] = ~_data[index];
        }

        return *this;
    }

    /**
     * @brief Flips the specified bit value (changes `true` value to `false` and vice versa).
     * @param index Index of the bit to flip.
     * @return Reference to this.
     */
    constexpr ibitset& flip(int index)
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index, " - ", size());

        _element(index) ^= _bit(index);
        return *this;
    }

    /**
     * @brief Returns the value stored at the specified index.
     */
    [[nodiscard]] constexpr bool operator[](int index) const
    {
        return test(index);
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr reference operator[](int index)
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index, " - ", size());

        return reference(*this, index);
    }

    /**
     * @brief Returns the value stored at the specified index.
     */
    [[nodiscard]] constexpr bool at(int index) const
    {
        return test(index);
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr reference at(int index)
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index, " - ", size());

        return reference(*this, index);
    }

    /**
     * @brief Indicates if all values are `true`.
     */
    [[nodiscard]] constexpr bool all() const
    {
        int last_element_index = _num_elements - 1;

        for(int index = last_element_index; index >= 0; --index)
        {
            if(_data[index] != _all_bits_set)
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Indicates if any value is `true`.
     */
    [[nodiscard]] constexpr bool any() const
    {
        return ! none();
    }

    /**
     * @brief Indicates if all values are `false`.
     */
    [[nodiscard]] constexpr bool none() const
    {
        int last_element_index = _num_elements - 1;

        for(int index = last_element_index; index >= 0; --index)
        {
            if(_data[index] != 0)
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Sets the bits to the result of binary AND of this ibitset and the given one.
     * @param other Another ibitset.
     * @return Reference to this.
     */
    constexpr ibitset& operator&=(const ibitset& other)
    {
        int num_elements = _num_elements;
        BN_BASIC_ASSERT(num_elements == other._num_elements, "Different size: ", size(), " - ", other.size());

        int last_element_index = num_elements - 1;

        for(int index = last_element_index; index >= 0; --index)
        {
            _data[index] &= other._data[index];
        }

        return *this;
    }

    /**
     * @brief Sets the bits to the result of binary OR of this ibitset and the given one.
     * @param other Another ibitset.
     * @return Reference to this.
     */
    constexpr ibitset& operator|=(const ibitset& other)
    {
        int num_elements = _num_elements;
        BN_BASIC_ASSERT(num_elements == other._num_elements, "Different size: ", size(), " - ", other.size());

        int last_element_index = num_elements - 1;

        for(int index = last_element_index; index >= 0; --index)
        {
            _data[index] |= other._data[index];
        }

        return *this;
    }

    /**
     * @brief Sets the bits to the result of binary XOR of this ibitset and the given one.
     * @param other Another ibitset.
     * @return Reference to this.
     */
    constexpr ibitset& operator^=(const ibitset& other)
    {
        int num_elements = _num_elements;
        BN_BASIC_ASSERT(num_elements == other._num_elements, "Different size: ", size(), " - ", other.size());

        int last_element_index = num_elements - 1;

        for(int index = last_element_index; index >= 0; --index)
        {
            _data[index] ^= other._data[index];
        }

        return *this;
    }

    /**
     * @brief Exchanges the contents of this ibitset with those of the other one.
     * @param other ibitset to exchange the contents with.
     */
    constexpr void swap(ibitset& other)
    {
        int num_elements = _num_elements;
        BN_BASIC_ASSERT(num_elements == other._num_elements, "Different size: ", size(), " - ", other.size());

        element_t* data = _data;
        swap_ranges(data, data + num_elements, other._data);
    }

    /**
     * @brief Exchanges the contents of an ibitset with those of another one.
     * @param a First ibitset to exchange the contents with.
     * @param b Second ibitset to exchange the contents with.
     */
    constexpr friend void swap(ibitset& a, ibitset& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First ibitset to compare.
     * @param b Second ibitset to compare.
     * @return `true` if the first ibitset is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const ibitset& a, const ibitset& b)
    {
        int num_elements = a._num_elements;

        if(num_elements != b._num_elements)
        {
            return false;
        }

        const element_t* a_data = a.data();
        const element_t* b_data = b.data();

        if(a_data == b_data)
        {
            return true;
        }

        return equal(a_data, a_data + num_elements, b_data);
    }

    /**
     * @brief Not equal operator.
     * @param a First ibitset to compare.
     * @param b Second ibitset to compare.
     * @return `true` if the first ibitset is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator!=(const ibitset& a, const ibitset& b)
    {
        return ! (a == b);
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    static constexpr int _bits_per_element = 8;

    constexpr ibitset(int num_elements, element_t* data) :
        _num_elements(num_elements),
        _data(data)
    {
    }

    /// @endcond

private:
    static constexpr element_t _all_bits_set = numeric_limits<element_t>::max();
    static constexpr element_t _all_bits_clear = 0;
    static constexpr int _bits_per_element_log2 = 3;

    int _num_elements;
    element_t* _data;

    [[nodiscard]] constexpr const element_t& _element(int index) const
    {
        return _data[index >> _bits_per_element_log2];
    }

    [[nodiscard]] constexpr element_t& _element(int index)
    {
        return _data[index >> _bits_per_element_log2];
    }

    [[nodiscard]] constexpr static element_t _bit(int index)
    {
        return element_t(1 << (index & (_bits_per_element - 1)));
    }
};


template<int Size>
class bitset : public ibitset
{
    static_assert(Size > 0);
    static_assert(Size % _bits_per_element == 0);

private:
    static constexpr int _num_elements = Size / _bits_per_element;

public:
    /**
     * @brief Default constructor.
     */
    constexpr bitset() :
        ibitset(_num_elements, _storage_buffer)
    {
    }

    /**
     * @brief Copy constructor.
     * @param other bitset to copy.
     */
    constexpr bitset(const bitset& other) :
        ibitset(_num_elements, _storage_buffer)
    {
        copy_n(other._storage_buffer, _num_elements, _storage_buffer);
    }

    /**
     * @brief Copy constructor.
     * @param other ibitset to copy.
     */
    constexpr bitset(const ibitset& other) :
        ibitset(_num_elements, _storage_buffer)
    {
        ibitset::operator=(other);
    }

    /**
     * @brief Copy assignment operator.
     * @param other bitset to copy.
     * @return Reference to this.
     */
    constexpr bitset& operator=(const bitset& other)
    {
        if(this != &other)
        {
            copy_n(other._storage_buffer, _num_elements, _storage_buffer);
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other ibitset to copy.
     * @return Reference to this.
     */
    constexpr bitset& operator=(const ibitset& other)
    {
        ibitset::operator=(other);
        return *this;
    }

    /**
     * @brief Sets all bits to `true`.
     * @return Reference to this.
     */
    constexpr bitset& set()
    {
        ibitset::set();
        return *this;
    }

    /**
     * @brief Sets the specified bit to `true`.
     * @param index Index of the bit to set.
     * @return Reference to this.
     */
    constexpr bitset& set(int index)
    {
        ibitset::set(index);
        return *this;
    }

    /**
     * @brief Sets the specified bit to the specified value.
     * @param index Index of the bit to set.
     * @param value Value to set.
     * @return Reference to this.
     */
    constexpr bitset& set(int index, bool value)
    {
        ibitset::set(index, value);
        return *this;
    }

    /**
     * @brief Sets all bits to `false`.
     * @return Reference to this.
     */
    constexpr bitset& reset()
    {
        ibitset::reset();
        return *this;
    }

    /**
     * @brief Sets the specified bit to `false`.
     * @param index Index of the bit to set.
     * @return Reference to this.
     */
    constexpr bitset& reset(int index)
    {
        ibitset::reset(index);
        return *this;
    }

    /**
     * @brief Flips all values (changes `true` values to `false` and vice versa).
     * @return Reference to this.
     */
    constexpr bitset& flip()
    {
        ibitset::flip();
        return *this;
    }

    /**
     * @brief Flips the specified bit value (changes `true` value to `false` and vice versa).
     * @param index Index of the bit to flip.
     * @return Reference to this.
     */
    constexpr bitset& flip(int index)
    {
        ibitset::flip(index);
        return *this;
    }

    /**
     * @brief Sets the bits to the result of binary AND of this ibitset and the given one.
     * @param other Another ibitset.
     * @return Reference to this.
     */
    constexpr bitset& operator&=(const ibitset& other)
    {
        ibitset::operator&=(other);
        return *this;
    }

    /**
     * @brief Sets the bits to the result of binary OR of this ibitset and the given one.
     * @param other Another ibitset.
     * @return Reference to this.
     */
    constexpr bitset& operator|=(const ibitset& other)
    {
        ibitset::operator|=(other);
        return *this;
    }

    /**
     * @brief Sets the bits to the result of binary XOR of this ibitset and the given one.
     * @param other Another ibitset.
     * @return Reference to this.
     */
    constexpr bitset& operator^=(const ibitset& other)
    {
        ibitset::operator^=(other);
        return *this;
    }

    /**
     * @brief Returns this bitset with all bits flipped (binary NOT).
     */
    [[nodiscard]] constexpr bitset operator~() const
    {
        bitset result = *this;
        result.flip();
        return result;
    }

    /**
     * @brief Returns a bitset containing the result of binary AND of the given bitsets.
     */
    [[nodiscard]] constexpr friend bitset operator&(const bitset& a, const bitset& b)
    {
        bitset result(a);
        result &= b;
        return result;
    }

    /**
     * @brief Returns a bitset containing the result of binary OR of the given bitsets.
     */
    [[nodiscard]] constexpr friend bitset operator|(const bitset& a, const bitset& b)
    {
        bitset result(a);
        result |= b;
        return result;
    }

    /**
     * @brief Returns a bitset containing the result of binary XOR of the given bitsets.
     */
    [[nodiscard]] constexpr friend bitset operator^(const bitset& a, const bitset& b)
    {
        bitset result(a);
        result ^= b;
        return result;
    }

private:
    alignas(int) element_t _storage_buffer[_num_elements] = {};
};


/**
 * @brief ibitset implementation which manages an external elements array.
 *
 * @ingroup bitset
 */
class bitset_ref : public ibitset
{

public:
    /**
     * @brief Default constructor.
     * @param elements_array_ref Elements array reference.
     *
     * The elements are not copied but referenced, so they should outlive the bitset_ref
     * to avoid dangling references.
     */
    template<int ElementSize>
    constexpr bitset_ref(element_t (&elements_array_ref)[ElementSize]) :
        ibitset(ElementSize, elements_array_ref)
    {
        static_assert(ElementSize >= 1);
    }

    /**
     * @brief Default constructor.
     * @param elements_ref Elements reference.
     *
     * The elements are not copied but referenced, so they should outlive the bitset_ref
     * to avoid dangling references.
     */
    constexpr bitset_ref(span<element_t> elements_ref) :
        ibitset(elements_ref.size(), elements_ref.data())
    {
        BN_BASIC_ASSERT(elements_size(), "No elements");
    }

    /**
     * @brief Copy assignment operator.
     * @param other ibitset to copy.
     * @return Reference to this.
     */
    constexpr bitset_ref& operator=(const ibitset& other)
    {
        ibitset::operator=(other);
        return *this;
    }

    /**
     * @brief Sets all bits to `true`.
     * @return Reference to this.
     */
    constexpr bitset_ref& set()
    {
        ibitset::set();
        return *this;
    }

    /**
     * @brief Sets the specified bit to `true`.
     * @param index Index of the bit to set.
     * @return Reference to this.
     */
    constexpr bitset_ref& set(int index)
    {
        ibitset::set(index);
        return *this;
    }

    /**
     * @brief Sets the specified bit to the specified value.
     * @param index Index of the bit to set.
     * @param value Value to set.
     * @return Reference to this.
     */
    constexpr bitset_ref& set(int index, bool value)
    {
        ibitset::set(index, value);
        return *this;
    }

    /**
     * @brief Sets all bits to `false`.
     * @return Reference to this.
     */
    constexpr bitset_ref& reset()
    {
        ibitset::reset();
        return *this;
    }

    /**
     * @brief Sets the specified bit to `false`.
     * @param index Index of the bit to set.
     * @return Reference to this.
     */
    constexpr bitset_ref& reset(int index)
    {
        ibitset::reset(index);
        return *this;
    }

    /**
     * @brief Flips all values (changes `true` values to `false` and vice versa).
     * @return Reference to this.
     */
    constexpr bitset_ref& flip()
    {
        ibitset::flip();
        return *this;
    }

    /**
     * @brief Flips the specified bit value (changes `true` value to `false` and vice versa).
     * @param index Index of the bit to flip.
     * @return Reference to this.
     */
    constexpr bitset_ref& flip(int index)
    {
        ibitset::flip(index);
        return *this;
    }

    /**
     * @brief Sets the bits to the result of binary AND of this ibitset and the given one.
     * @param other Another ibitset.
     * @return Reference to this.
     */
    constexpr bitset_ref& operator&=(const ibitset& other)
    {
        ibitset::operator&=(other);
        return *this;
    }

    /**
     * @brief Sets the bits to the result of binary OR of this ibitset and the given one.
     * @param other Another ibitset.
     * @return Reference to this.
     */
    constexpr bitset_ref& operator|=(const ibitset& other)
    {
        ibitset::operator|=(other);
        return *this;
    }

    /**
     * @brief Sets the bits to the result of binary XOR of this ibitset and the given one.
     * @param other Another ibitset.
     * @return Reference to this.
     */
    constexpr bitset_ref& operator^=(const ibitset& other)
    {
        ibitset::operator^=(other);
        return *this;
    }
};

}

#endif
