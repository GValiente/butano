#ifndef BTN_HASH_MAP_H
#define BTN_HASH_MAP_H

#include <new>
#include "btn_memory.h"
#include "btn_algorithm.h"
#include "btn_power_of_two.h"
#include "btn_hash_map_fwd.h"

namespace btn
{

template<typename Key, typename Value, typename KeyHash, typename KeyEqual>
class ihash_map
{

public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = pair<const key_type, mapped_type>;
    using size_type = size_t;
    using hasher = KeyHash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    class iterator
    {

    public:
        using key_type = ihash_map::key_type;
        using mapped_type = ihash_map::mapped_type;
        using value_type = ihash_map::value_type;
        using size_type = ihash_map::size_type;
        using hasher = ihash_map::hasher;
        using key_equal = ihash_map::key_equal;
        using reference = ihash_map::reference;
        using const_reference = ihash_map::const_reference;
        using pointer = ihash_map::pointer;
        using const_pointer = ihash_map::const_pointer;

        friend class ihash_map;

        iterator& operator++()
        {
            size_type index = _index;
            size_type last_valid_index = _map->_last_valid_index;
            const bool* allocated = _map->_allocated;
            ++index;

            while(index <= last_valid_index && ! allocated[index])
            {
                ++index;
            }

            if(index > last_valid_index)
            {
                index = _map->max_size();
            }

            _index = index;
            return *this;
        }

        [[nodiscard]] iterator operator++(int)
        {
            iterator result(*this);
            ++result;
            return result;
        }

        iterator& operator--()
        {
            int index = _index;
            int first_valid_index = _map->_first_valid_index;
            const bool* allocated = _map->_allocated;
            --index;

            while(index >= first_valid_index && ! allocated[index])
            {
                --index;
            }

            _index = index;
            return *this;
        }

        [[nodiscard]] iterator operator--(int)
        {
            iterator result(*this);
            --result;
            return result;
        }

        [[nodiscard]] const_reference operator*() const
        {
            BTN_ASSERT(_map->_allocated[_index], "Index is not allocated: ", _index);

            return _map->_storage[_index];
        }

        [[nodiscard]] reference operator*()
        {
            BTN_ASSERT(_map->_allocated[_index], "Index is not allocated: ", _index);

            return _map->_storage[_index];
        }

        [[nodiscard]] const_pointer operator&() const
        {
            BTN_ASSERT(_map->_allocated[_index], "Index is not allocated: ", _index);

            return _map->_storage + _index;
        }

        [[nodiscard]] pointer operator&()
        {
            BTN_ASSERT(_map->_allocated[_index], "Index is not allocated: ", _index);

            return _map->_storage + _index;
        }

        const_pointer operator->() const
        {
            BTN_ASSERT(_map->_allocated[_index], "Index is not allocated: ", _index);

            return _map->_storage + _index;
        }

        pointer operator->()
        {
            BTN_ASSERT(_map->_allocated[_index], "Index is not allocated: ", _index);

            return _map->_storage + _index;
        }

        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b)
        {
            return  a._index == b._index;
        }

        [[nodiscard]] friend bool operator!=(const iterator& a, const iterator& b)
        {
            return ! (a == b);
        }

    private:
        size_type _index;
        ihash_map* _map;

        iterator(size_type index, ihash_map& map) :
            _index(index),
            _map(&map)
        {
        }
    };

    class const_iterator
    {

    public:
        using key_type = ihash_map::key_type;
        using mapped_type = ihash_map::mapped_type;
        using value_type = ihash_map::value_type;
        using size_type = ihash_map::size_type;
        using hasher = ihash_map::hasher;
        using key_equal = ihash_map::key_equal;
        using reference = ihash_map::reference;
        using const_reference = ihash_map::const_reference;
        using pointer = ihash_map::pointer;
        using const_pointer = ihash_map::const_pointer;

        friend class ihash_map;
        friend class iterator;

        const_iterator(const iterator& other):
            _index(other._index),
            _map(other._map)
        {
        }

        const_iterator& operator++()
        {
            size_type index = _index;
            size_type last_valid_index = _map->_last_valid_index;
            const bool* allocated = _map->_allocated;
            ++index;

            while(index <= last_valid_index && ! allocated[index])
            {
                ++index;
            }

            if(index > last_valid_index)
            {
                index = _map->max_size();
            }

            _index = index;
            return *this;
        }

        [[nodiscard]] const_iterator operator++(int)
        {
            const_iterator result(*this);
            ++result;
            return result;
        }

        const_iterator& operator--()
        {
            int index = _index;
            int first_valid_index = _map->_first_valid_index;
            const bool* allocated = _map->_allocated;
            --index;

            while(index >= first_valid_index && ! allocated[index])
            {
                --index;
            }

            _index = index;
            return *this;
        }

        [[nodiscard]] const_iterator operator--(int)
        {
            const_iterator result(*this);
            --result;
            return result;
        }

        [[nodiscard]] const_reference operator*() const
        {
            BTN_ASSERT(_map->_allocated[_index], "Index is not allocated: ", _index);

            return _map->_storage[_index];
        }

        [[nodiscard]] const_pointer operator&() const
        {
            BTN_ASSERT(_map->_allocated[_index], "Index is not allocated: ", _index);

            return _map->_storage + _index;
        }

        const_pointer operator->() const
        {
            BTN_ASSERT(_map->_allocated[_index], "Index is not allocated: ", _index);

            return _map->_storage + _index;
        }

        [[nodiscard]] friend bool operator==(const const_iterator& a, const const_iterator& b)
        {
            return  a._index == b._index;
        }

        [[nodiscard]] friend bool operator!=(const const_iterator& a, const const_iterator& b)
        {
            return ! (a == b);
        }

    private:
        size_type _index;
        const ihash_map* _map;

        const_iterator(size_type index, const ihash_map& map) :
            _index(index),
            _map(&map)
        {
        }
    };

    friend class iterator;
    friend class const_iterator;

    [[nodiscard]] iterator begin()
    {
        return iterator(_first_valid_index, *this);
    }

    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(_first_valid_index, *this);
    }

    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(_first_valid_index, *this);
    }

    [[nodiscard]] iterator end()
    {
        return iterator(_max_size, *this);
    }

    [[nodiscard]] const_iterator end() const
    {
        return const_iterator(_max_size, *this);
    }

    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator(_max_size, *this);
    }

    [[nodiscard]] size_type size() const
    {
        return _size;
    }

    [[nodiscard]] size_type max_size() const
    {
        return _max_size;
    }

    [[nodiscard]] size_type available() const
    {
        return _max_size - _size;
    }

    [[nodiscard]] bool empty() const
    {
        return _size == 0;
    }

    [[nodiscard]] bool full() const
    {
        return _size == _max_size;
    }

    [[nodiscard]] bool contains(const key_type& key) const
    {
        if(empty())
        {
            return false;
        }

        return contains_hash(hasher()(key), key);
    }

    [[nodiscard]] bool contains_hash(size_type key_hash, const key_type& key) const
    {
        return find_hash(key_hash, key) != end();
    }

    [[nodiscard]] size_type count(const key_type& key) const
    {
        return count_hash(hasher()(key), key);
    }

    [[nodiscard]] size_type count_hash(size_type key_hash, const key_type& key) const
    {
        return contains_hash(key_hash, key) ? 1 : 0;
    }

    [[nodiscard]] const_iterator find(const key_type& key) const
    {
        return const_cast<ihash_map&>(*this).find(key);
    }

    [[nodiscard]] iterator find(const key_type& key)
    {
        if(empty())
        {
            return end();
        }

        return find_hash(hasher()(key), key);
    }

    [[nodiscard]] const_iterator find_hash(size_type key_hash, const key_type& key) const
    {
        return const_cast<ihash_map&>(*this).find_hash(key_hash, key);
    }

    [[nodiscard]] iterator find_hash(size_type key_hash, const key_type& key)
    {
        if(empty())
        {
            return end();
        }

        const_pointer storage = _storage;
        const bool* allocated = _allocated;
        key_equal key_equal_functor;
        size_type index = _index(key_hash);
        size_type max_size = _max_size;
        size_type its = 0;

        while(its < max_size && allocated[index])
        {
            if(key_equal_functor(key, storage[index].first))
            {
                return iterator(index, *this);
            }

            index = _index(index + 1);
            ++its;
        }

        return end();
    }

    [[nodiscard]] const mapped_type& at(const key_type& key) const
    {
        return const_cast<ihash_map&>(*this).at_hash(key);
    }

    [[nodiscard]] mapped_type& at(const key_type& key)
    {
        return at_hash(hasher()(key), key);
    }

    [[nodiscard]] const mapped_type& at_hash(size_type key_hash, const key_type& key) const
    {
        return const_cast<ihash_map&>(*this).at_hash(key_hash, key);
    }

    [[nodiscard]] mapped_type& at_hash(size_type key_hash, const key_type& key)
    {
        iterator it = find_hash(key_hash, key);
        BTN_ASSERT(it != end(), "Key not found");

        return it->second;
    }

    iterator insert(const value_type& value)
    {
        return insert_hash(hasher()(value.first), value);
    }

    iterator insert(value_type&& value)
    {
        return insert_hash(hasher()(value.first), move(value));
    }

    template<typename KeyType, typename ValueType>
    iterator insert(KeyType&& key, ValueType&& value)
    {
        return insert_hash(hasher()(key), value_type(forward<KeyType>(key), forward<ValueType>(value)));
    }

    iterator insert_hash(size_type key_hash, const value_type& value)
    {
        return insert_hash(key_hash, value_type(value));
    }

    iterator insert_hash(size_type key_hash, value_type&& value)
    {
        BTN_ASSERT(! full(), "Hash map is full");

        size_type index = _index(key_hash);
        pointer storage = _storage;
        bool* allocated = _allocated;
        key_equal key_equal_functor;
        size_type current_index = index;

        while(allocated[current_index])
        {
            if(key_equal_functor(value.first, storage[current_index].first))
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

    template<typename KeyType, typename ValueType>
    iterator insert_hash(size_type key_hash, KeyType&& key, ValueType&& value)
    {
        return insert_hash(key_hash, value_type(forward<KeyType>(key), forward<ValueType>(value)));
    }

    iterator insert_or_assign(const value_type& value)
    {
        return insert_or_assign_hash(hasher()(value.first), value);
    }

    iterator insert_or_assign(value_type&& value)
    {
        return insert_or_assign_hash(hasher()(value.first), move(value));
    }

    template<typename KeyType, typename ValueType>
    iterator insert_or_assign(KeyType&& key, ValueType&& value)
    {
        return insert_or_assign_hash(hasher()(key), value_type(forward<KeyType>(key), forward<ValueType>(value)));
    }

    iterator insert_or_assign_hash(size_type key_hash, const value_type& value)
    {
        return insert_or_assign_hash(key_hash, value_type(value));
    }

    iterator insert_or_assign_hash(size_type key_hash, value_type&& value)
    {
        iterator it = find_hash(key_hash, value.first);

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

    template<typename KeyType, typename ValueType>
    iterator insert_or_assign_hash(size_type key_hash, KeyType&& key, ValueType&& value)
    {
        return insert_or_assign_hash(key_hash, value_type(forward<KeyType>(key), forward<ValueType>(value)));
    }

    iterator erase(const_iterator pos)
    {
        BTN_ASSERT(! empty(), "Hash map is empty");

        bool* allocated = _allocated;
        size_type index = pos._index;
        BTN_ASSERT(allocated[index], "Index is not allocated: ", index);

        pointer storage = _storage;
        storage[index].~value_type();
        allocated[index] = false;
        --_size;

        if(_size == 0)
        {
            _first_valid_index = _max_size;
            _last_valid_index = 0;
            return end();
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

        hasher hasher_functor;
        size_type current_index = index;
        size_type next_index = _index(index + 1);

        while(allocated[next_index] && _index(hasher_functor(storage[next_index].first)) != next_index)
        {
            ::new(storage + current_index) value_type(move(storage[next_index]));
            storage[next_index].~value_type();
            allocated[current_index] = true;
            allocated[next_index] = false;
            current_index = next_index;
            next_index = _index(next_index + 1);
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

    bool erase_hash(size_type key_hash, const key_type& key)
    {
        iterator it = find_hash(key_hash, key);

        if(it != end())
        {
            erase(it);
            return true;
        }

        return false;
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

            size_type max_size = _max_size;
            memory::clear(int(max_size), *allocated);
            _first_valid_index = max_size;
            _last_valid_index = 0;
            _size = 0;
        }
    }

    [[nodiscard]] mapped_type& operator[](const key_type& key)
    {
        return operator()(hasher()(key), key);
    }

    [[nodiscard]] mapped_type& operator()(const key_type& key)
    {
        return operator()(hasher()(key), key);
    }

    [[nodiscard]] mapped_type& operator()(size_type key_hash, const key_type& key)
    {
        iterator it = find_hash(key_hash, key);

        if(it == end())
        {
            it = insert_hash(key_hash, key, mapped_type());
            BTN_ASSERT(it != end(), "Insertion failed");
        }

        return it->second;
    }

    [[nodiscard]] friend bool operator==(const ihash_map& a, const ihash_map& b)
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

    [[nodiscard]] bool friend operator!=(const ihash_map& a, const ihash_map& b)
    {
        return ! (a == b);
    }

protected:
    ihash_map(reference storage, bool& allocated, size_type max_size) :
        _storage(&storage),
        _allocated(&allocated),
        _max_size(max_size),
        _first_valid_index(max_size)
    {
        BTN_ASSERT(power_of_two(max_size), "Max size is not power of two: ", max_size);
    }

private:
    pointer _storage;
    bool* _allocated;
    size_type _max_size;
    size_type _first_valid_index;
    size_type _last_valid_index = 0;
    size_type _size = 0;

    void _assign(const ihash_map& other)
    {
        const_pointer other_storage = other._typed_storage();
        pointer storage = _storage;
        bool* allocated = _allocated;
        size_type first_valid_index = other._first_valid_index;
        size_type last_valid_index = other._last_valid_index;
        memory::copy(*other._allocated, _max_size, *allocated);

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

    void _assign(ihash_map&& other)
    {
        pointer other_storage = other._typed_storage();
        pointer storage = _storage;
        bool* allocated = _allocated;
        size_type first_valid_index = other._first_valid_index;
        size_type last_valid_index = other._last_valid_index;
        memory::copy(*other._allocated, _max_size, *allocated);

        for(size_type index = first_valid_index; index <= last_valid_index; ++index)
        {
            if(allocated[index])
            {
                ::new(storage + index) value_type(move(other_storage[index]));
            }
        }

        _first_valid_index = other._first_valid_index;
        _last_valid_index = other._last_valid_index;
        _size = other._size;

        other.clear();
    }

    size_type _index(size_type key_hash) const
    {
        return key_hash & (_max_size - 1);
    }
};


template<typename Key, typename Value, size_t MaxSize, typename KeyHash, typename KeyEqual>
class hash_map : public ihash_map<Key, Value, KeyHash, KeyEqual>
{
    static_assert(power_of_two(MaxSize));

public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = pair<const key_type, mapped_type>;
    using size_type = size_t;
    using hasher = KeyHash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    hash_map() :
        ihash_map<Key, Value, KeyHash, KeyEqual>(*reinterpret_cast<pointer>(_storage_buffer), *_allocated_buffer,
                                                 MaxSize)
    {
    }

    hash_map(const hash_map& other)
    {
        _assign(other);
    }

    hash_map& operator=(const hash_map& other)
    {
        if(this != &other)
        {
            this->clear();
            _assign(other);
        }

        return *this;
    }

    hash_map(hash_map&& other)
    {
        _assign(move(other));
    }

    hash_map& operator=(hash_map&& other)
    {
        if(this != &other)
        {
            this->clear();
            _assign(move(other));
        }

        return *this;
    }

    ~hash_map()
    {
        this->clear();
    }

private:
    alignas(alignof(value_type)) char _storage_buffer[sizeof(value_type) * MaxSize];
    bool _allocated_buffer[MaxSize] = { false };
};

}

#endif
