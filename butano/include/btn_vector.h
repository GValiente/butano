#ifndef BTN_VECTOR_H
#define BTN_VECTOR_H

#include <new>
#include "btn_assert.h"
#include "btn_iterator.h"
#include "btn_algorithm.h"
#include "btn_vector_fwd.h"

namespace btn
{

template<typename Type>
class ivector
{

public:
    using value_type = Type;
    using size_type = int;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;
    using iterator = Type*;
    using const_iterator = const Type*;
    using reverse_iterator = btn::reverse_iterator<iterator>;
    using const_reverse_iterator = btn::reverse_iterator<const_iterator>;

    ivector(const ivector& other) = delete;

    ivector& operator=(const ivector& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other._size <= _max_size, "Not enough space in vector: ", _max_size, " - ", other._size);

            clear();
            _assign(other);
        }

        return *this;
    }

    ivector& operator=(ivector&& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other._size <= _max_size, "Not enough space in vector: ", _max_size, " - ", other._size);

            clear();
            _assign(move(other));
        }

        return *this;
    }

    [[nodiscard]] const_pointer data() const
    {
        return _data;
    }

    [[nodiscard]] pointer data()
    {
        return _data;
    }

    [[nodiscard]] size_type size() const
    {
        return _size;
    }

    [[nodiscard]] size_type max_size() const
    {
        return _max_size;
    }

    [[nodiscard]] bool empty() const
    {
        return _size == 0;
    }

    [[nodiscard]] bool full() const
    {
        return _size == _max_size;
    }

    [[nodiscard]] size_type available() const
    {
        return _max_size - _size;
    }

    [[nodiscard]] const_iterator begin() const
    {
        return _data;
    }

    [[nodiscard]] iterator begin()
    {
        return _data;
    }

    [[nodiscard]] const_iterator end() const
    {
        return _data + _size;
    }

    [[nodiscard]] iterator end()
    {
        return _data + _size;
    }

    [[nodiscard]] const_iterator cbegin() const
    {
        return _data;
    }

    [[nodiscard]] const_iterator cend() const
    {
        return _data + _size;
    }

    [[nodiscard]] const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    [[nodiscard]] const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]] reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    [[nodiscard]] const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    [[nodiscard]] const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    [[nodiscard]] const_reference operator[](size_type index) const
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    [[nodiscard]] reference operator[](size_type index)
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    [[nodiscard]] const_reference at(size_type index) const
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    [[nodiscard]] reference at(size_type index)
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    [[nodiscard]] const_reference front() const
    {
        BTN_ASSERT(_size, "Vector is empty");

        return _data[0];
    }

    [[nodiscard]] reference front()
    {
        BTN_ASSERT(_size, "Vector is empty");

        return _data[0];
    }

    [[nodiscard]] const_reference back() const
    {
        BTN_ASSERT(_size, "Vector is empty");

        return _data[_size - 1];
    }

    [[nodiscard]] reference back()
    {
        BTN_ASSERT(_size, "Vector is empty");

        return _data[_size - 1];
    }

    void push_back(const_reference value)
    {
        BTN_ASSERT(! full(), "Vector is full");

        ::new(_data + _size) value_type(value);
        ++_size;
    }

    void push_back(value_type&& value)
    {
        BTN_ASSERT(! full(), "Vector is full");

        ::new(_data + _size) value_type(move(value));
        ++_size;
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        BTN_ASSERT(! full(), "Vector is full");

        ::new(_data + _size) value_type(forward<Args>(args)...);
        ++_size;
    }

    void pop_back()
    {
        BTN_ASSERT(_size, "Vector is empty");

        --_size;
        _data[_size].~value_type();
    }

    iterator insert(const_iterator position, const_reference value)
    {
        BTN_ASSERT(position >= begin() && position <= end(), "Invalid position");
        BTN_ASSERT(! full(), "Vector is full");

        auto non_const_position = const_cast<iterator>(position);
        iterator last = end();
        ::new(_data + _size) value_type(value);
        ++_size;

        for(iterator it = non_const_position; it != last; ++it)
        {
            btn::swap(*it, *last);
        }

        return non_const_position;
    }

    iterator insert(const_iterator position, value_type&& value)
    {
        BTN_ASSERT(position >= begin() && position <= end(), "Invalid position");
        BTN_ASSERT(! full(), "Vector is full");

        auto non_const_position = const_cast<iterator>(position);
        iterator last = end();
        ::new(_data + _size) value_type(move(value));
        ++_size;

        for(iterator it = non_const_position; it != last; ++it)
        {
            btn::swap(*it, *last);
        }

        return non_const_position;
    }

    template<typename... Args>
    iterator emplace(const_iterator position, Args&&... args)
    {
        BTN_ASSERT(position >= begin() && position <= end(), "Invalid position");
        BTN_ASSERT(! full(), "Vector is full");

        auto non_const_position = const_cast<iterator>(position);
        iterator last = end();
        ::new(_data + _size) value_type(forward<Args>(args)...);
        ++_size;

        for(iterator it = non_const_position; it != last; ++it)
        {
            btn::swap(*it, *last);
        }

        return non_const_position;
    }

    iterator erase(const_iterator position)
    {
        BTN_ASSERT(_size, "Vector is empty");
        BTN_ASSERT(position >= begin() && position < end(), "Invalid position");

        auto non_const_position = const_cast<iterator>(position);
        iterator it = non_const_position;
        --_size;

        iterator last = end();

        while(it != last)
        {
            iterator next = it + 1;
            *it = move(*next);
            it = next;
        }

        _data[_size].~value_type();
        return non_const_position;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        BTN_ASSERT(first >= begin(), "Invalid first");
        BTN_ASSERT(last <= end(), "Invalid last");

        size_type delete_count = last - first;
        BTN_ASSERT(delete_count >= 0 && delete_count <= _size, "Invalid delete count: ", delete_count, " - ", _size);

        if(delete_count)
        {
            if(delete_count == _size)
            {
                clear();
            }
            else
            {
                iterator erase_it =  const_cast<iterator>(first);
                iterator erase_next = erase_it + delete_count;
                iterator erase_last = end();
                _size -= delete_count;

                while(erase_next != erase_last)
                {
                    *erase_it = move(*erase_next);
                    ++erase_it;
                    ++erase_next;
                }

                while(erase_it != erase_last)
                {
                    erase_it->~value_type();
                    ++erase_it;
                }
            }
        }

        return const_cast<iterator>(first);
    }

    friend void erase(ivector& vector, const_reference value)
    {
        vector.erase(remove(vector.begin(), vector.end(), value), vector.end());
    }

    template<class Pred>
    friend void erase_if(ivector& vector, const Pred& pred)
    {
        vector.erase(remove_if(vector.begin(), vector.end(), pred), vector.end());
    }

    void resize(size_type count)
    {
        BTN_ASSERT(count >= 0 && count <= _max_size, "Invalid count: ", count, " - ", _max_size);

        pointer data = _data;
        size_type size = _size;

        if(count < size)
        {
            for(size_type index = count; index < size; ++index)
            {
                data[index].~value_type();
            }
        }
        else
        {
            for(size_type index = size; index < count; ++index)
            {
                ::new(data + index) value_type();
            }
        }

        _size = count;
    }

    void resize(size_type count, const_reference value)
    {
        BTN_ASSERT(count >= 0 && count <= _max_size, "Invalid count: ", count, " - ", _max_size);

        pointer data = _data;
        size_type size = _size;

        if(count < size)
        {
            for(size_type index = count; index < size; ++index)
            {
                data[index].~value_type();
            }
        }
        else
        {
            for(size_type index = size; index < count; ++index)
            {
                ::new(data + index) value_type(value);
            }
        }

        _size = count;
    }

    void assign(size_type count, const_reference value)
    {
        BTN_ASSERT(count >= 0 && count <= _max_size, "Invalid count: ", count, " - ", _max_size);

        pointer data = _data;
        clear();
        _size = count;

        for(size_type index = 0; index < count; ++index)
        {
            ::new(data + index) value_type(value);
        }
    }

    template<typename Iterator>
    void assign(const Iterator& first, const Iterator& last)
    {
        size_type count = last - first;
        BTN_ASSERT(count >= 0 && count <= _max_size, "Invalid count: ", count, " - ", _max_size);

        pointer data = _data;
        clear();
        _size = count;

        for(size_type index = 0; index < count; ++index)
        {
            ::new(data + index) value_type(*first);
            ++first;
        }
    }

    void swap(ivector& other)
    {
        if(_data != other._data)
        {
            BTN_ASSERT(_size <= other._max_size, "Invalid size: ", _size, " - ", other._max_size);
            BTN_ASSERT(_max_size <= other._size, "Invalid max size: ", _max_size, " - ", other._size);

            pointer min_data;
            pointer max_data;
            size_type min_size;
            size_type max_size;

            if(_size < other._size)
            {
                min_data = _data;
                max_data = other._data;
                min_size = _size;
                max_size = other._size;
            }
            else
            {
                min_data = other._data;
                max_data = _data;
                min_size = other._size;
                max_size = _size;
            }

            for(size_type index = 0; index < min_size; ++index)
            {
                btn::swap(min_data[index], max_data[index]);
            }

            for(size_type index = min_size; index < max_size; ++index)
            {
                min_data[index] = move(max_data[index]);
                max_data[index].~value_type();
            }

            btn::swap(_size, other._size);
        }
    }

    friend void swap(ivector& a, ivector& b)
    {
        a.swap(b);
    }

    void clear()
    {
        pointer data = _data;

        for(size_type index = 0, size = _size; index < size; ++index)
        {
            data[index].~value_type();
        }

        _size = 0;
    }

    [[nodiscard]] friend bool operator==(const ivector& a, const ivector& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] friend bool operator!=(const ivector& a, const ivector& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] friend bool operator<(const ivector& a, const ivector& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] friend bool operator>(const ivector& a, const ivector& b)
    {
        return b < a;
    }

    [[nodiscard]] friend bool operator<=(const ivector& a, const ivector& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] friend bool operator>=(const ivector& a, const ivector& b)
    {
        return ! (a < b);
    }

protected:
    pointer _data;
    size_type _size;
    size_type _max_size;

    ivector(reference data, size_type max_size) :
        _data(&data),
        _size(0),
        _max_size(max_size)
    {
    }

    void _assign(const ivector& other)
    {
        pointer data = _data;
        const_pointer other_data = other._data;
        size_type other_size = other._size;
        _size = other_size;

        for(size_type index = 0; index < other_size; ++index)
        {
            ::new(data + index) value_type(other_data[index]);
        }
    }

    void _assign(ivector&& other)
    {
        pointer data = _data;
        pointer other_data = other._data;
        size_type other_size = other._size;
        _size = other_size;

        for(size_type index = 0; index < other_size; ++index)
        {
            value_type& other_value = other_data[index];
            ::new(data + index) value_type(move(other_value));
            other_value.~value_type();
        }

        other._size = 0;
    }
};


template<typename Type, int MaxSize>
class vector : public ivector<Type>
{
    static_assert(MaxSize > 0);

public:
    using value_type = Type;
    using size_type = int;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;
    using iterator = Type*;
    using const_iterator = const Type*;
    using reverse_iterator = btn::reverse_iterator<iterator>;
    using const_reverse_iterator = btn::reverse_iterator<const_iterator>;

    vector() :
        ivector<Type>(*reinterpret_cast<pointer>(_storage_buffer), MaxSize)
    {
    }

    vector(const vector& other) :
        vector()
    {
        this->_assign(other);
    }

    vector& operator=(const vector& other)
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    vector(vector&& other) :
        vector()
    {
        this->_assign(move(other));
    }

    vector& operator=(vector&& other)
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    ~vector()
    {
        this->clear();
    }

private:
    alignas(alignof(value_type)) char _storage_buffer[sizeof(value_type) * MaxSize];
};

}

#endif
