#ifndef BTN_HASH_SET_H
#define BTN_HASH_SET_H

#include <new>
#include "btn_memory.h"
#include "btn_iterator.h"
#include "btn_algorithm.h"
#include "btn_power_of_two.h"
#include "btn_hash_set_fwd.h"

namespace btn
{

template<typename Key, typename KeyHash, typename KeyEqual>
class ihash_set
{

public:
    using key_type = Key;
    using value_type = Key;
    using size_type = int;
    using hash_type = unsigned;
    using hasher = KeyHash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    class iterator
    {

    public:
        using key_type = ihash_set::key_type;
        using value_type = ihash_set::value_type;
        using size_type = ihash_set::size_type;
        using hash_type = ihash_set::hash_type;
        using hasher = ihash_set::hasher;
        using key_equal = ihash_set::key_equal;
        using reference = ihash_set::reference;
        using const_reference = ihash_set::const_reference;
        using pointer = ihash_set::pointer;
        using const_pointer = ihash_set::const_pointer;
        using iterator_category = bidirectional_iterator_tag;

        iterator& operator++()
        {
            size_type index = _index;
            size_type last_valid_index = _set->_last_valid_index;
            const bool* allocated = _set->_allocated;
            ++index;

            while(index <= last_valid_index && ! allocated[index])
            {
                ++index;
            }

            if(index > last_valid_index)
            {
                index = _set->max_size();
            }

            _index = index;
            return *this;
        }

        iterator& operator--()
        {
            int index = _index;
            int first_valid_index = _set->_first_valid_index;
            const bool* allocated = _set->_allocated;
            --index;

            while(index >= first_valid_index && ! allocated[index])
            {
                --index;
            }

            _index = index;
            return *this;
        }

        [[nodiscard]] const_reference operator*() const
        {
            BTN_ASSERT(_set->_allocated[_index], "Index is not allocated: ", _index);

            return _set->_storage[_index];
        }

        [[nodiscard]] reference operator*()
        {
            BTN_ASSERT(_set->_allocated[_index], "Index is not allocated: ", _index);

            return _set->_storage[_index];
        }

        const_pointer operator->() const
        {
            BTN_ASSERT(_set->_allocated[_index], "Index is not allocated: ", _index);

            return _set->_storage + _index;
        }

        pointer operator->()
        {
            BTN_ASSERT(_set->_allocated[_index], "Index is not allocated: ", _index);

            return _set->_storage + _index;
        }

        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b)
        {
            return a._index == b._index;
        }

        [[nodiscard]] friend bool operator!=(const iterator& a, const iterator& b)
        {
            return ! (a == b);
        }

    private:
        friend class ihash_set;

        size_type _index;
        ihash_set* _set;

        iterator(size_type index, ihash_set& set) :
            _index(index),
            _set(&set)
        {
        }
    };

    class const_iterator
    {

    public:
        using key_type = ihash_set::key_type;
        using value_type = ihash_set::value_type;
        using size_type = ihash_set::size_type;
        using hash_type = ihash_set::hash_type;
        using hasher = ihash_set::hasher;
        using key_equal = ihash_set::key_equal;
        using reference = ihash_set::reference;
        using const_reference = ihash_set::const_reference;
        using pointer = ihash_set::pointer;
        using const_pointer = ihash_set::const_pointer;
        using iterator_category = bidirectional_iterator_tag;

        const_iterator(const iterator& other) :
            _index(other._index),
            _set(other._set)
        {
        }

        const_iterator& operator++()
        {
            size_type index = _index;
            size_type last_valid_index = _set->_last_valid_index;
            const bool* allocated = _set->_allocated;
            ++index;

            while(index <= last_valid_index && ! allocated[index])
            {
                ++index;
            }

            if(index > last_valid_index)
            {
                index = _set->max_size();
            }

            _index = index;
            return *this;
        }

        const_iterator& operator--()
        {
            int index = _index;
            int first_valid_index = _set->_first_valid_index;
            const bool* allocated = _set->_allocated;
            --index;

            while(index >= first_valid_index && ! allocated[index])
            {
                --index;
            }

            _index = index;
            return *this;
        }

        [[nodiscard]] const_reference operator*() const
        {
            BTN_ASSERT(_set->_allocated[_index], "Index is not allocated: ", _index);

            return _set->_storage[_index];
        }

        const_pointer operator->() const
        {
            BTN_ASSERT(_set->_allocated[_index], "Index is not allocated: ", _index);

            return _set->_storage + _index;
        }

        [[nodiscard]] friend bool operator==(const const_iterator& a, const const_iterator& b)
        {
            return a._index == b._index;
        }

        [[nodiscard]] friend bool operator!=(const const_iterator& a, const const_iterator& b)
        {
            return ! (a == b);
        }

    private:
        friend class ihash_set;
        friend class iterator;

        size_type _index;
        const ihash_set* _set;

        const_iterator(size_type index, const ihash_set& set) :
            _index(index),
            _set(&set)
        {
        }
    };

    ihash_set(const ihash_set& other) = delete;

    ihash_set& operator=(const ihash_set& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other._size <= max_size(), "Not enough space in hash set: ", max_size(), " - ", other._size);

            clear();
            _assign(other);
        }

        return *this;
    }

    ihash_set& operator=(ihash_set&& other) noexcept
    {
        if(this != &other)
        {
            BTN_ASSERT(other._size <= max_size(), "Not enough space in hash set: ", max_size(), " - ", other._size);

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

    [[nodiscard]] size_type available() const
    {
        return max_size() - _size;
    }

    [[nodiscard]] bool empty() const
    {
        return _size == 0;
    }

    [[nodiscard]] bool full() const
    {
        return _size == max_size();
    }

    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(_first_valid_index, *this);
    }

    [[nodiscard]] iterator begin()
    {
        return iterator(_first_valid_index, *this);
    }

    [[nodiscard]] const_iterator end() const
    {
        return const_iterator(max_size(), *this);
    }

    [[nodiscard]] iterator end()
    {
        return iterator(max_size(), *this);
    }

    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(_first_valid_index, *this);
    }

    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator(max_size(), *this);
    }

    [[nodiscard]] bool contains(const key_type& key) const
    {
        if(empty())
        {
            return false;
        }

        return contains_hash(hasher()(key), key);
    }

    [[nodiscard]] bool contains_hash(hash_type key_hash, const key_type& key) const
    {
        return find_hash(key_hash, key) != end();
    }

    [[nodiscard]] size_type count(const key_type& key) const
    {
        return count_hash(hasher()(key), key);
    }

    [[nodiscard]] size_type count_hash(hash_type key_hash, const key_type& key) const
    {
        return contains_hash(key_hash, key) ? 1 : 0;
    }

    [[nodiscard]] const_iterator find(const key_type& key) const
    {
        return const_cast<ihash_set&>(*this).find(key);
    }

    [[nodiscard]] iterator find(const key_type& key)
    {
        if(empty())
        {
            return end();
        }

        return find_hash(hasher()(key), key);
    }

    [[nodiscard]] const_iterator find_hash(hash_type key_hash, const key_type& key) const
    {
        return const_cast<ihash_set&>(*this).find_hash(key_hash, key);
    }

    [[nodiscard]] iterator find_hash(hash_type key_hash, const key_type& key)
    {
        if(empty())
        {
            return end();
        }

        const_pointer storage = _storage;
        const bool* allocated = _allocated;
        key_equal key_equal_functor;
        size_type index = _index(key_hash);
        size_type max_size = _max_size_minus_one + 1;
        size_type its = 0;

        while(its < max_size && allocated[index])
        {
            if(key_equal_functor(key, storage[index]))
            {
                return iterator(index, *this);
            }

            index = _index(index + 1);
            ++its;
        }

        return end();
    }

    iterator insert(const value_type& value)
    {
        return insert_hash(hasher()(value), value);
    }

    iterator insert(value_type&& value)
    {
        return insert_hash(hasher()(value), move(value));
    }

    iterator insert_hash(hash_type key_hash, const value_type& value)
    {
        return insert_hash(key_hash, value_type(value));
    }

    iterator insert_hash(hash_type key_hash, value_type&& value)
    {
        BTN_ASSERT(! full(), "Hash set is full");

        size_type index = _index(key_hash);
        pointer storage = _storage;
        bool* allocated = _allocated;
        key_equal key_equal_functor;
        size_type current_index = index;

        while(allocated[current_index])
        {
            if(key_equal_functor(value, storage[current_index]))
            {
                return end();
            }

            current_index = _index(current_index + 1);
            BTN_ASSERT(current_index != index, "All indices are allocated");
        }

        ::new(storage + current_index) value_type(move(value));
        allocated[current_index] = true;
        _first_valid_index = min(_first_valid_index, current_index);
        _last_valid_index = max(_last_valid_index, current_index);
        ++_size;
        return iterator(current_index, *this);
    }

    iterator insert_or_assign(const value_type& value)
    {
        return insert_or_assign_hash(hasher()(value), value);
    }

    iterator insert_or_assign(value_type&& value)
    {
        return insert_or_assign_hash(hasher()(value), move(value));
    }

    iterator insert_or_assign_hash(hash_type key_hash, const value_type& value)
    {
        return insert_or_assign_hash(key_hash, value_type(value));
    }

    iterator insert_or_assign_hash(hash_type key_hash, value_type&& value)
    {
        iterator it = find_hash(key_hash, value);

        if(it == end())
        {
            it = insert_hash(key_hash, move(value));
            BTN_ASSERT(it != end(), "Insertion failed");
        }
        else
        {
            pointer storage = _storage;
            size_type index = it._index;
            storage[index].~value_type();
            ::new(storage + index) value_type(move(value));
        }

        return it;
    }

    iterator erase(const const_iterator& position)
    {
        BTN_ASSERT(! empty(), "Hash set is empty");

        bool* allocated = _allocated;
        size_type index = position._index;
        BTN_ASSERT(allocated[index], "Index is not allocated: ", index);

        pointer storage = _storage;
        storage[index].~value_type();
        allocated[index] = false;
        --_size;

        if(_size == 0)
        {
            _first_valid_index = max_size();
            _last_valid_index = 0;
            return end();
        }

        hasher hasher_functor;
        size_type current_index = index;
        size_type next_index = _index(index + 1);

        while(allocated[next_index] && _index(hasher_functor(storage[next_index])) != next_index)
        {
            ::new(storage + current_index) value_type(move(storage[next_index]));
            storage[next_index].~value_type();
            allocated[current_index] = true;
            allocated[next_index] = false;
            current_index = next_index;
            next_index = _index(next_index + 1);
        }

        if(_size == 1)
        {
            if(index == _first_valid_index)
            {
                index = _last_valid_index;
                _first_valid_index = _last_valid_index;
            }
            else
            {
                index = _first_valid_index;
                _last_valid_index = _first_valid_index;
            }

            return iterator(index, *this);
        }

        size_type first_valid_index = _first_valid_index;

        while(! allocated[first_valid_index])
        {
            ++first_valid_index;
        }

        _first_valid_index = first_valid_index;

        size_type last_valid_index = _last_valid_index;

        while(! allocated[last_valid_index])
        {
            --last_valid_index;
        }

        _last_valid_index = last_valid_index;

        while(index <= last_valid_index && ! allocated[index])
        {
            ++index;
        }

        return iterator(index, *this);
    }

    bool erase(const key_type& key)
    {
        return erase_hash(hasher()(key), key);
    }

    bool erase_hash(hash_type key_hash, const key_type& key)
    {
        iterator it = find_hash(key_hash, key);

        if(it != end())
        {
            erase(it);
            return true;
        }

        return false;
    }

    template<class Pred>
    friend void erase_if(ihash_set& hash_set, const Pred& pred)
    {
        pointer storage = hash_set._storage;
        bool* allocated = hash_set._allocated;
        size_type size = hash_set._size;
        size_type first_valid_index = hash_set.max_size();
        size_type last_valid_index = 0;

        for(size_type index = hash_set._first_valid_index, last = hash_set._last_valid_index; index <= last; ++index)
        {
            if(allocated[index])
            {
                if(allocated[index] && pred(storage[index]))
                {
                    storage[index].~value_type();
                    allocated[index] = false;
                    --size;
                }
                else
                {
                    first_valid_index = min(index, first_valid_index);
                    last_valid_index = max(index, last_valid_index);
                }
            }
        }

        hash_set._size = size;
        hash_set._first_valid_index = first_valid_index;
        hash_set._last_valid_index = last_valid_index;
    }

    void merge(ihash_set&& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(_max_size_minus_one == other._max_size_minus_one,
                       "Invalid max size: ", max_size(), " - ", other.max_size());

            pointer storage = _storage;
            pointer other_storage = other._storage;
            bool* allocated = _allocated;
            bool* other_allocated = other._allocated;
            size_type size = _size;
            size_type first_valid_index = min(_first_valid_index, other._first_valid_index);
            size_type last_valid_index = max(_last_valid_index, other._last_valid_index);

            for(size_type index = first_valid_index; index <= last_valid_index; ++index)
            {
                if(other_allocated[index])
                {
                    value_type&& other_value = other_storage[index];

                    if(allocated[index])
                    {
                        storage[index] = move(other_value);
                    }
                    else
                    {
                        ::new(storage + index) value_type(move(other_value));
                        ++size;
                    }

                    other_value.~value_type();
                }
            }

            _size = size;
            _first_valid_index = first_valid_index;
            _last_valid_index = last_valid_index;

            int other_max_size = other.max_size();
            memory::clear(other_max_size, *other.allocated);
            other._first_valid_index = other_max_size;
            other._last_valid_index = 0;
            other._size = 0;
        }
    }

    void clear()
    {
        if(_size)
        {
            pointer storage = _storage;
            bool* allocated = _allocated;
            size_type first_valid_index = _first_valid_index;
            size_type last_valid_index = _last_valid_index;

            for(size_type index = first_valid_index; index <= last_valid_index; ++index)
            {
                if(allocated[index])
                {
                    storage[index].~value_type();
                }
            }

            size_type max_size = _max_size_minus_one + 1;
            memory::clear(max_size, *allocated);
            _first_valid_index = max_size;
            _last_valid_index = 0;
            _size = 0;
        }
    }

    void swap(ihash_set& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(_max_size_minus_one == other._max_size_minus_one,
                       "Invalid max size: ", max_size(), " - ", other.max_size());

            pointer storage = _storage;
            pointer other_storage = other._storage;
            bool* allocated = _allocated;
            bool* other_allocated = other._allocated;
            size_type first_valid_index = min(_first_valid_index, other._first_valid_index);
            size_type last_valid_index = max(_last_valid_index, other._last_valid_index);

            for(size_type index = first_valid_index; index <= last_valid_index; ++index)
            {
                if(other_allocated[index])
                {
                    if(allocated[index])
                    {
                        btn::swap(storage[index], other_storage[index]);
                    }
                    else
                    {
                        ::new(storage + index) value_type(move(other_storage[index]));
                        other_storage[index].~value_type();
                        other_allocated[index] = false;
                        allocated[index] = true;
                    }
                }
                else
                {
                    if(allocated[index])
                    {
                        ::new(other_storage + index) value_type(move(storage[index]));
                        storage[index].~value_type();
                        allocated[index] = false;
                        other_allocated[index] = true;
                    }
                }
            }

            btn::swap(_size, other._size);
            btn::swap(_first_valid_index, other._first_valid_index);
            btn::swap(_last_valid_index, other._last_valid_index);
        }
    }

    friend void swap(ihash_set& a, ihash_set& b)
    {
        a.swap(b);
    }

    [[nodiscard]] friend bool operator==(const ihash_set& a, const ihash_set& b)
    {
        size_type first_valid_index = a._first_valid_index;
        size_type last_valid_index = a._last_valid_index;

        if(a._size != b._size || first_valid_index != b._first_valid_index || last_valid_index != b._last_valid_index)
        {
            return false;
        }

        const_pointer a_storage = a._storage;
        const_pointer b_storage = b._storage;
        const bool* a_allocated = a._allocated;
        const bool* b_allocated = b._allocated;

        for(size_type index = first_valid_index; index <= last_valid_index; ++index)
        {
            if(a_allocated[index] != b_allocated[index])
            {
                return false;
            }

            if(a_allocated[index] && a_storage[index] != b_storage[index])
            {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] friend bool operator!=(const ihash_set& a, const ihash_set& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] friend bool operator<(const ihash_set& a, const ihash_set& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] friend bool operator>(const ihash_set& a, const ihash_set& b)
    {
        return b < a;
    }

    [[nodiscard]] friend bool operator<=(const ihash_set& a, const ihash_set& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] friend bool operator>=(const ihash_set& a, const ihash_set& b)
    {
        return ! (a < b);
    }

protected:
    ihash_set(reference storage, bool& allocated, size_type max_size) :
        _storage(&storage),
        _allocated(&allocated),
        _max_size_minus_one(max_size - 1),
        _first_valid_index(max_size)
    {
        BTN_ASSERT(power_of_two(max_size), "Max size is not power of two: ", max_size);
    }

private:
    pointer _storage;
    bool* _allocated;
    size_type _max_size_minus_one;
    size_type _first_valid_index;
    size_type _last_valid_index = 0;
    size_type _size = 0;

    void _assign(const ihash_set& other)
    {
        const_pointer other_storage = other._storage;
        pointer storage = _storage;
        bool* allocated = _allocated;
        size_type first_valid_index = other._first_valid_index;
        size_type last_valid_index = other._last_valid_index;
        memory::copy(*other._allocated, other.max_size(), *allocated);

        for(size_type index = first_valid_index; index <= last_valid_index; ++index)
        {
            if(allocated[index])
            {
                ::new(storage + index) value_type(other_storage[index]);
            }
        }

        _first_valid_index = other._first_valid_index;
        _last_valid_index = other._last_valid_index;
        _size = other._size;
    }

    void _assign(ihash_set&& other)
    {
        pointer other_storage = other._storage;
        pointer storage = _storage;
        bool* allocated = _allocated;
        size_type first_valid_index = other._first_valid_index;
        size_type last_valid_index = other._last_valid_index;
        int other_max_size = other.max_size();
        memory::copy(*other._allocated, other_max_size, *allocated);

        for(size_type index = first_valid_index; index <= last_valid_index; ++index)
        {
            if(allocated[index])
            {
                value_type& other_value = other_storage[index];
                ::new(storage + index) value_type(move(other_value));
                other_value.~value_type();
            }
        }

        _first_valid_index = other._first_valid_index;
        _last_valid_index = other._last_valid_index;
        _size = other._size;

        memory::clear(other_max_size, *other.allocated);
        other._first_valid_index = other_max_size;
        other._last_valid_index = 0;
        other._size = 0;
    }

    [[nodiscard]] size_type _index(hash_type key_hash) const
    {
        return key_hash & _max_size_minus_one;
    }
};


template<typename Key, int MaxSize, typename KeyHash, typename KeyEqual>
class hash_set : public ihash_set<Key, KeyHash, KeyEqual>
{
    static_assert(power_of_two(MaxSize));

public:
    using key_type = Key;
    using value_type = Key;
    using size_type = int;
    using hash_type = unsigned;
    using hasher = KeyHash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    hash_set() :
        ihash_set<Key, KeyHash, KeyEqual>(*reinterpret_cast<pointer>(_storage_buffer), *_allocated_buffer, MaxSize)
    {
    }

    hash_set(const hash_set& other) :
        hash_set()
    {
        this->_assign(other);
    }

    hash_set(hash_set&& other) noexcept :
        hash_set()
    {
        this->_assign(move(other));
    }

    hash_set(const ihash_set<Key, KeyHash, KeyEqual>& other) :
        hash_set()
    {
        BTN_ASSERT(other.size() <= MaxSize, "Not enough space in hash set: ", MaxSize, " - ", other.size());

        this->_assign(other);
    }

    hash_set(ihash_set<Key, KeyHash, KeyEqual>&& other) noexcept :
        hash_set()
    {
        BTN_ASSERT(other.size() <= MaxSize, "Not enough space in hash set: ", MaxSize, " - ", other.size());

        this->_assign(move(other));
    }

    hash_set& operator=(const hash_set& other)
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    hash_set& operator=(hash_set&& other) noexcept
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    hash_set& operator=(const ihash_set<Key, KeyHash, KeyEqual>& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other.size() <= MaxSize, "Not enough space in hash set: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    hash_set& operator=(ihash_set<Key, KeyHash, KeyEqual>&& other) noexcept
    {
        if(this != &other)
        {
            BTN_ASSERT(other.size() <= MaxSize, "Not enough space in hash set: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    ~hash_set()
    {
        this->clear();
    }

private:
    alignas(alignof(value_type)) char _storage_buffer[sizeof(value_type) * MaxSize];
    bool _allocated_buffer[MaxSize] = {};
};

}

#endif
