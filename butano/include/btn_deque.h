#ifndef BTN_DEQUE_H
#define BTN_DEQUE_H

#include <new>
#include "btn_assert.h"
#include "btn_iterator.h"
#include "btn_algorithm.h"
#include "btn_power_of_two.h"
#include "btn_deque_fwd.h"

namespace btn
{

template<typename Type>
class ideque
{

public:
    using value_type = Type;
    using size_type = int;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;

    class iterator
    {

    public:
        iterator& operator++()
        {
            ++_index;
            return *this;
        }

        iterator& operator+=(size_type value)
        {
            _index += value;
            return *this;
        }

        iterator& operator+=(iterator other)
        {
            _index += other._index;
            return *this;
        }

        iterator& operator--()
        {
            --_index;
            return *this;
        }

        iterator& operator-=(size_type value)
        {
            _index -= value;
            return *this;
        }

        iterator& operator-=(iterator other)
        {
            _index -= other._index;
            return *this;
        }

        [[nodiscard]] const_reference operator*() const
        {
            return _deque->at(_index);
        }

        [[nodiscard]] reference operator*()
        {
            return _deque->at(_index);
        }

        const_pointer operator->() const
        {
            return &_deque->at(_index);
        }

        pointer operator->()
        {
            return &_deque->at(_index);
        }

        [[nodiscard]] friend iterator operator+(const iterator& a, size_type b)
        {
            return iterator(&a._deque, a._index + b);
        }

        [[nodiscard]] friend iterator operator+(const iterator& a, const iterator& b)
        {
            return iterator(&a._deque, a._index + b._index);
        }

        [[nodiscard]] friend iterator operator-(const iterator& a, size_type b)
        {
            return iterator(&a._deque, a._index - b);
        }

        [[nodiscard]] friend iterator operator-(const iterator& a, const iterator& b)
        {
            return iterator(&a._deque, a._index - b._index);
        }

        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b)
        {
            return a._index == b._index;
        }

        [[nodiscard]] friend bool operator!=(const iterator& a, const iterator& b)
        {
            return ! (a == b);
        }

        [[nodiscard]] friend bool operator<(const iterator& a, const iterator& b)
        {
            return a._index < b._index;
        }

        [[nodiscard]] friend bool operator>(const iterator& a, const iterator& b)
        {
            return b < a;
        }

        [[nodiscard]] friend bool operator<=(const iterator& a, const iterator& b)
        {
            return ! (a > b);
        }

        [[nodiscard]] friend bool operator>=(const iterator& a, const iterator& b)
        {
            return ! (a < b);
        }

    private:
        friend class ideque;
        friend class const_iterator;

        ideque* _deque;
        size_type _index;

        iterator(ideque& deque, size_type index) :
            _deque(&deque),
            _index(index)
        {
        }
    };

    class const_iterator
    {

    public:
        const_iterator(iterator it) :
            _deque(it._deque),
            _index(it._index)
        {
        }

        const_iterator& operator++()
        {
            ++_index;
            return *this;
        }

        const_iterator& operator+=(size_type value)
        {
            _index += value;
            return *this;
        }

        const_iterator& operator+=(const const_iterator& other)
        {
            _index += other._index;
            return *this;
        }

        const_iterator& operator--()
        {
            --_index;
            return *this;
        }

        const_iterator& operator-=(size_type value)
        {
            _index -= value;
            return *this;
        }

        const_iterator& operator-=(const const_iterator& other)
        {
            _index -= other._index;
            return *this;
        }

        [[nodiscard]] const_reference operator*() const
        {
            return _deque->at(_index);
        }

        const_pointer operator->() const
        {
            return &_deque->at(_index);
        }

        [[nodiscard]] friend const_iterator operator+(const const_iterator& a, size_type b)
        {
            return const_iterator(&a._deque, a._index + b);
        }

        [[nodiscard]] friend const_iterator operator+(const const_iterator& a, const const_iterator& b)
        {
            return const_iterator(&a._deque, a._index + b._index);
        }

        [[nodiscard]] friend const_iterator operator-(const const_iterator& a, size_type b)
        {
            return const_iterator(&a._deque, a._index - b);
        }

        [[nodiscard]] friend const_iterator operator-(const const_iterator& a, const const_iterator& b)
        {
            return const_iterator(&a._deque, a._index - b._index);
        }

        [[nodiscard]] friend bool operator==(const const_iterator& a, const const_iterator& b)
        {
            return a._index == b._index;
        }

        [[nodiscard]] friend bool operator!=(const const_iterator& a, const const_iterator& b)
        {
            return ! (a == b);
        }

        [[nodiscard]] friend bool operator<(const const_iterator& a, const const_iterator& b)
        {
            return a._index < b._index;
        }

        [[nodiscard]] friend bool operator>(const const_iterator& a, const const_iterator& b)
        {
            return b < a;
        }

        [[nodiscard]] friend bool operator<=(const const_iterator& a, const const_iterator& b)
        {
            return ! (a > b);
        }

        [[nodiscard]] friend bool operator>=(const const_iterator& a, const const_iterator& b)
        {
            return ! (a < b);
        }

    private:
        friend class ideque;

        const ideque* _deque;
        size_type _index;

        const_iterator(const ideque& deque, size_type index) :
            _deque(&deque),
            _index(index)
        {
        }
    };

    using reverse_iterator = btn::reverse_iterator<iterator>;
    using const_reverse_iterator = btn::reverse_iterator<const_iterator>;

    ideque(const ideque& other) = delete;

    ideque& operator=(const ideque& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other._size <= max_size(), "Not enough space in deque: ", max_size(), " - ", other._size);

            clear();
            _assign(other);
        }

        return *this;
    }

    ideque& operator=(ideque&& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other._size <= max_size(), "Not enough space in deque: ", max_size(), " - ", other._size);

            clear();
            _assign(move(other));
        }

        return *this;
    }

    [[nodiscard]] size_type size() const
    {
        return _size;
    }

    [[nodiscard]] size_type max_size() const
    {
        return _max_size_minus_one + 1;
    }

    [[nodiscard]] bool empty() const
    {
        return _size == 0;
    }

    [[nodiscard]] bool full() const
    {
        return _size == max_size();
    }

    [[nodiscard]] size_type available() const
    {
        return max_size() - _size;
    }

    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(*this, 0);
    }

    [[nodiscard]] iterator begin()
    {
        return iterator(*this, 0);
    }

    [[nodiscard]] const_iterator end() const
    {
        return const_iterator(*this, _size);
    }

    [[nodiscard]] iterator end()
    {
        return iterator(*this, _size);
    }

    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(*this, 0);
    }

    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator(*this, _size);
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

        return _value(index);
    }

    [[nodiscard]] reference operator[](size_type index)
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _value(index);
    }

    [[nodiscard]] const_reference at(size_type index) const
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _value(index);
    }

    [[nodiscard]] reference at(size_type index)
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _value(index);
    }

    [[nodiscard]] const_reference front() const
    {
        BTN_ASSERT(_size, "Deque is empty");

        return _data[_begin];
    }

    [[nodiscard]] reference front()
    {
        BTN_ASSERT(_size, "Deque is empty");

        return _data[_begin];
    }

    [[nodiscard]] const_reference back() const
    {
        BTN_ASSERT(_size, "Deque is empty");

        return _value(_size - 1);
    }

    [[nodiscard]] reference back()
    {
        BTN_ASSERT(_size, "Deque is empty");

        return _value(_size - 1);
    }

    void push_back(const_reference value)
    {
        BTN_ASSERT(! full(), "Deque is full");

        ::new(_data + _real_index(_size)) value_type(value);
        ++_size;
    }

    void push_back(value_type&& value)
    {
        BTN_ASSERT(! full(), "Deque is full");

        ::new(_data + _real_index(_size)) value_type(move(value));
        ++_size;
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        BTN_ASSERT(! full(), "Deque is full");

        ::new(_data + _real_index(_size)) value_type(forward<Args>(args)...);
        ++_size;
    }

    void pop_back()
    {
        BTN_ASSERT(_size, "Deque is empty");

        --_size;
        _value(_size).~value_type();
    }

    void push_front(const_reference value)
    {
        BTN_ASSERT(! full(), "Deque is full");

        _begin = (_begin - 1) & _max_size_minus_one;
        ::new(_data + _begin) value_type(value);
        ++_size;
    }

    void push_front(value_type&& value)
    {
        BTN_ASSERT(! full(), "Deque is full");

        _begin = (_begin - 1) & _max_size_minus_one;
        ::new(_data + _begin) value_type(move(value));
        ++_size;
    }

    template<typename... Args>
    void emplace_front(Args&&... args)
    {
        BTN_ASSERT(! full(), "Deque is full");

        _begin = (_begin - 1) & _max_size_minus_one;
        ::new(_data + _begin) value_type(forward<Args>(args)...);
        ++_size;
    }

    void pop_front()
    {
        BTN_ASSERT(_size, "Deque is empty");

        _pop_front();
    }

    iterator insert(const const_iterator& position, const_reference value)
    {
        size_type index = position._index;

        if(index == 0)
        {
            push_front(value);
        }
        else
        {
            BTN_ASSERT(index >= 0 && index <= _size, "Invalid position: ", index, " - ", _size);
            BTN_ASSERT(! full(), "Deque is full");

            pointer data = _data;
            size_type last = _size;
            size_type last_real_index = _real_index(last);
            ::new(data + last_real_index) value_type(value);
            reference last_value = data[last_real_index];

            for(; index != last; ++index)
            {
                btn::swap(data[_real_index(index)], last_value);
            }

            ++_size;
        }

        return _mutable_iterator(position);
    }

    iterator insert(const const_iterator& position, value_type&& value)
    {
        size_type index = position._index;

        if(index == 0)
        {
            push_front(move(value));
        }
        else
        {
            BTN_ASSERT(index >= 0 && index <= _size, "Invalid position: ", index, " - ", _size);
            BTN_ASSERT(! full(), "Deque is full");

            pointer data = _data;
            size_type last = _size;
            size_type last_real_index = _real_index(last);
            ::new(data + last_real_index) value_type(move(value));
            reference last_value = data[last_real_index];

            for(; index != last; ++index)
            {
                btn::swap(data[_real_index(index)], last_value);
            }

            ++_size;
        }

        return _mutable_iterator(position);
    }

    template<typename... Args>
    iterator emplace(const const_iterator& position, Args&&... args)
    {
        size_type index = position._index;

        if(index == 0)
        {
            emplace_front(forward<Args>(args)...);
        }
        else
        {
            BTN_ASSERT(index >= 0 && index <= _size, "Invalid position: ", index, " - ", _size);
            BTN_ASSERT(! full(), "Deque is full");

            pointer data = _data;
            size_type last = _size;
            size_type last_real_index = _real_index(last);
            ::new(_data + last_real_index) value_type(forward<Args>(args)...);
            reference last_value = data[last_real_index];

            for(; index != last; ++index)
            {
                btn::swap(data[_real_index(index)], last_value);
            }

            ++_size;
        }

        return _mutable_iterator(position);
    }

    iterator erase(const const_iterator& position)
    {
        size_type index = position._index;

        if(index == 0)
        {
            pop_front();
        }
        else
        {
            BTN_ASSERT(index >= 0 && index < _size, "Invalid position: ", index, " - ", _size);

            --_size;

            pointer data = _data;
            size_type last = _size;
            size_type real_index = _real_index(index);

            for(; index != last; ++index)
            {
                size_type real_next_index = _real_index(index + 1);
                data[real_index] = move(data[real_next_index]);
                real_index = real_next_index;
            }

            data[real_index].~value_type();
        }

        return _mutable_iterator(position);
    }

    iterator erase(const const_iterator& first, const const_iterator& last)
    {
        size_type first_index = first._index;
        BTN_ASSERT(first_index >= 0 && first_index < _size, "Invalid first: ", first_index, " - ", _size);

        size_type last_index = last._index;
        BTN_ASSERT(last_index >= 0 && last_index < _size, "Invalid last: ", last_index, " - ", _size);

        size_type delete_count = last_index - first_index;
        BTN_ASSERT(delete_count >= 0 && delete_count <= _size, "Invalid delete count: ", delete_count, " - ", _size);

        if(delete_count)
        {
            if(delete_count == _size)
            {
                clear();
            }
            else if(first_index == 0)
            {
                while(delete_count)
                {
                    _pop_front();
                    --delete_count;
                }
            }
            else
            {
                pointer data = _data;
                size_type index = first_index;
                size_type next_index = first_index + delete_count;
                size_type last_index = _size;
                _size -= delete_count;

                while(next_index != last_index)
                {
                    data[_real_index(index)] = move(data[_real_index(next_index)]);
                    ++index;
                    ++next_index;
                }

                while(index != last_index)
                {
                    data[_real_index(index)].~value_type();
                    ++index;
                }
            }
        }

        return _mutable_iterator(first);
    }

    friend void erase(ideque& deque, const_reference value)
    {
        deque.erase(remove(deque.begin(), deque.end(), value), deque.end());
    }

    template<class Pred>
    friend void erase_if(ideque& deque, const Pred& pred)
    {
        deque.erase(remove_if(deque.begin(), deque.end(), pred), deque.end());
    }

    void resize(size_type count)
    {
        BTN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        pointer data = _data;
        size_type size = _size;
        _size = count;

        if(count < size)
        {
            for(size_type index = count; index < size; ++index)
            {
                data[_real_index(index)].~value_type();
            }
        }
        else
        {
            for(size_type index = size; index < count; ++index)
            {
                ::new(data + _real_index(index)) value_type();
            }
        }
    }

    void resize(size_type count, const_reference value)
    {
        BTN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        pointer data = _data;
        size_type size = _size;
        _size = count;

        if(count < size)
        {
            for(size_type index = count; index < size; ++index)
            {
                data[_real_index(index)].~value_type();
            }
        }
        else
        {
            for(size_type index = size; index < count; ++index)
            {
                ::new(data + _real_index(index)) value_type(value);
            }
        }
    }

    void shrink(size_type count)
    {
        BTN_ASSERT(count >= 0 && count <= _size, "Invalid count: ", count, " - ", _size);

        pointer data = _data;

        for(size_type index = count, limit = _size; index < limit; ++index)
        {
            data[_real_index(index)].~value_type();
        }

        _size = count;
    }

    void assign(size_type count, const_reference value)
    {
        BTN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

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
        BTN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        pointer data = _data;
        clear();
        _size = count;

        for(size_type index = 0; index < count; ++index)
        {
            ::new(data + index) value_type(*first);
            ++first;
        }
    }

    void swap(ideque& other)
    {
        if(_data != other._data)
        {
            BTN_ASSERT(_size <= other.max_size(), "Invalid size: ", _size, " - ", other.max_size());
            BTN_ASSERT(max_size() <= other._size, "Invalid max size: ", max_size(), " - ", other._size);

            ideque* min_deque;
            ideque* max_deque;

            if(_size < other._size)
            {
                min_deque = this;
                max_deque = &other;
            }
            else
            {
                min_deque = &other;
                max_deque = this;
            }

            pointer min_data = min_deque->_data;
            pointer max_data = max_deque->_data;
            size_type min_size = min_deque->_size;
            size_type max_size = max_deque->_size;

            for(size_type index = 0; index < min_size; ++index)
            {
                btn::swap(min_data[min_deque->_real_index(index)], max_data[max_deque->_real_index(index)]);
            }

            for(size_type index = min_size; index < max_size; ++index)
            {
                size_type max_real_index = max_deque->_real_index(index);
                min_data[min_deque->_real_index(index)] = move(max_data[max_real_index]);
                max_data[max_real_index].~value_type();
            }

            btn::swap(_size, other._size);
        }
    }

    friend void swap(ideque& a, ideque& b)
    {
        a.swap(b);
    }

    void clear()
    {
        pointer data = _data;

        for(size_type index = 0, size = _size; index < size; ++index)
        {
            data[_real_index(index)].~value_type();
        }

        _size = 0;
        _begin = 0;
    }

    [[nodiscard]] friend bool operator==(const ideque& a, const ideque& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] friend bool operator!=(const ideque& a, const ideque& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] friend bool operator<(const ideque& a, const ideque& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] friend bool operator>(const ideque& a, const ideque& b)
    {
        return b < a;
    }

    [[nodiscard]] friend bool operator<=(const ideque& a, const ideque& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] friend bool operator>=(const ideque& a, const ideque& b)
    {
        return ! (a < b);
    }

protected:
    ideque(reference data, size_type max_size) :
        _data(&data),
        _size(0),
        _max_size_minus_one(max_size - 1),
        _begin(0)
    {
    }

    void _assign(const ideque& other)
    {
        pointer data = _data;
        const_pointer other_data = other._data;
        size_type other_size = other._size;
        _size = other_size;

        for(size_type index = 0; index < other_size; ++index)
        {
            ::new(data + index) value_type(other_data[other._real_index(index)]);
        }
    }

    void _assign(ideque&& other)
    {
        pointer data = _data;
        pointer other_data = other._data;
        size_type other_size = other._size;
        _size = other_size;

        for(size_type index = 0; index < other_size; ++index)
        {
            value_type& other_value = other_data[other._real_index(index)];
            ::new(data + index) value_type(move(other_value));
            other_value.~value_type();
        }

        other._size = 0;
        other._begin = 0;
    }

    void _assign(size_type count)
    {
        pointer data = _data;
        size_type size = _size;
        _size = count;

        for(size_type index = size; index < count; ++index)
        {
            ::new(data + index) value_type();
        }
    }

    void _assign(size_type count, const_reference value)
    {
        pointer data = _data;
        _size = count;

        for(size_type index = 0; index < count; ++index)
        {
            ::new(data + index) value_type(value);
        }
    }

private:
    pointer _data;
    size_type _size;
    size_type _max_size_minus_one;
    size_type _begin;

    [[nodiscard]] static iterator _mutable_iterator(const const_iterator& const_iterator)
    {
        return iterator(const_cast<ideque&>(*const_iterator._deque), const_iterator._index);
    }

    [[nodiscard]] size_type _real_index(size_type index) const
    {
        return (_begin + index) & _max_size_minus_one;
    }

    [[nodiscard]] const_reference _value(size_type index) const
    {
        return _data[_real_index(index)];
    }

    [[nodiscard]] reference _value(size_type index)
    {
        return _data[_real_index(index)];
    }

    void _pop_front()
    {
        _data[_size].~value_type();
        --_size;
        _begin = (_begin + 1) & _max_size_minus_one;
    }
};


template<typename Type, int MaxSize>
class deque : public ideque<Type>
{
    static_assert(power_of_two(MaxSize));

public:
    using value_type = Type;
    using size_type = int;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;
    using iterator = typename ideque<Type>::iterator;
    using const_iterator = typename ideque<Type>::const_iterator;
    using reverse_iterator = typename ideque<Type>::reverse_iterator;
    using const_reverse_iterator = typename ideque<Type>::const_reverse_iterator;

    deque() :
        ideque<Type>(*reinterpret_cast<pointer>(_storage_buffer), MaxSize)
    {
    }

    deque(const deque& other) :
        deque()
    {
        this->_assign(other);
    }

    deque(deque&& other) :
        deque()
    {
        this->_assign(move(other));
    }

    deque(const ideque<Type>& other) :
        deque()
    {
        BTN_ASSERT(other.size() <= MaxSize, "Not enough space in deque: ", MaxSize, " - ", other.size());

        this->_assign(other);
    }

    deque(ideque<Type>&& other) :
        deque()
    {
        BTN_ASSERT(other.size() <= MaxSize, "Not enough space in deque: ", MaxSize, " - ", other.size());

        this->_assign(move(other));
    }

    deque(size_type count) :
        deque()
    {
        BTN_ASSERT(count >= 0 && count <= MaxSize, "Invalid count: ", count, " - ", MaxSize);

        this->_assign(count);
    }

    deque(size_type count, const_reference value) :
        deque()
    {
        BTN_ASSERT(count >= 0 && count <= MaxSize, "Invalid count: ", count, " - ", MaxSize);

        this->_assign(count, value);
    }

    deque& operator=(const deque& other)
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    deque& operator=(deque&& other)
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    deque& operator=(const ideque<Type>& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other.size() <= MaxSize, "Not enough space in deque: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    deque& operator=(ideque<Type>&& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other.size() <= MaxSize, "Not enough space in deque: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    ~deque()
    {
        this->clear();
    }

private:
    alignas(alignof(value_type)) char _storage_buffer[sizeof(value_type) * MaxSize];
};

}

#endif
