/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UNORDERED_SET_H
#define BN_UNORDERED_SET_H

/**
 * @file
 * bn::iunordered_set and bn::unordered_set implementation header file.
 *
 * @ingroup unordered_set
 */

#include <new>
#include "bn_memory.h"
#include "bn_iterator.h"
#include "bn_algorithm.h"
#include "bn_power_of_two.h"
#include "bn_unordered_set_fwd.h"

namespace bn
{

template<typename Key, typename KeyHash, typename KeyEqual>
class iunordered_set
{

public:
    using key_type = Key; //!< Key type alias.
    using value_type = Key; //!< Value type alias.
    using size_type = int; //!< Size type alias.
    using difference_type = int; //!< Difference type alias.
    using hash_type = unsigned; //!< Hash type alias.
    using hasher = KeyHash; //!< Hash functor alias.
    using key_equal = KeyEqual; //!< Equality functor alias.
    using reference = value_type&; //!< Reference alias.
    using const_reference = const value_type&; //!< Const reference alias.
    using pointer = value_type*; //!< Pointer alias.
    using const_pointer = const value_type*; //!< Const pointer alias.

    /**
     * @brief Non const iterator.
     *
     * In contrast to std::unordered_set::iterator, this one is bidirectional.
     */
    class iterator
    {

    public:
        using key_type = iunordered_set::key_type; //!< Key type alias.
        using value_type = iunordered_set::value_type; //!< Value type alias.
        using size_type = iunordered_set::size_type; //!< Size type alias.
        using difference_type = iunordered_set::difference_type; //!< Difference type alias.
        using hash_type = iunordered_set::hash_type; //!< Hash type alias.
        using hasher = iunordered_set::hasher; //!< Hash functor alias.
        using key_equal = iunordered_set::key_equal; //!< Equality functor alias.
        using reference = iunordered_set::reference; //!< Reference alias.
        using pointer = iunordered_set::pointer; //!< Pointer alias.
        using iterator_category = bidirectional_iterator_tag; //!< Iterator category alias.

        /**
         * @brief Default constructor.
         */
        iterator() = default;

        /**
         * @brief Increments the position.
         * @return Reference to this.
         */
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

        /**
         * @brief Increments the position.
         * @return The iterator before being incremented.
         */
        iterator operator++(int)
        {
            iterator copy(*this);
            operator++();
            return copy;
        }

        /**
         * @brief Decrements the position.
         * @return Reference to this.
         */
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

        /**
         * @brief Decrements the position.
         * @return The iterator before being decremented.
         */
        iterator operator--(int)
        {
            iterator copy(*this);
            operator--();
            return copy;
        }

        /**
         * @brief Returns a reference to the pointed value.
         */
        [[nodiscard]] reference operator*() const
        {
            return _set->_storage[_index];
        }

        /**
         * @brief Returns a pointer to the pointed value.
         */
        pointer operator->() const
        {
            return _set->_storage + _index;
        }

        /**
         * @brief Equal operator.
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return `true` if the first iterator is equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b)
        {
            return a._index == b._index;
        }

        /**
         * @brief Not equal operator.
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return `true` if the first iterator is not equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator!=(const iterator& a, const iterator& b)
        {
            return a._index != b._index;
        }

    private:
        friend class iunordered_set;

        size_type _index = 0;
        iunordered_set* _set = nullptr;

        iterator(size_type index, iunordered_set& set) :
            _index(index),
            _set(&set)
        {
        }
    };

    /**
     * @brief Const iterator.
     *
     * In contrast to std::unordered_set::const_iterator, this one is bidirectional.
     */
    class const_iterator
    {

    public:
        using key_type = iunordered_set::key_type; //!< Key type alias.
        using value_type = iunordered_set::value_type; //!< Value type alias.
        using size_type = iunordered_set::size_type; //!< Size type alias.
        using difference_type = iunordered_set::difference_type; //!< Difference type alias.
        using hash_type = iunordered_set::hash_type; //!< Hash type alias.
        using hasher = iunordered_set::hasher; //!< Hash functor alias.
        using key_equal = iunordered_set::key_equal; //!< Equality functor alias.
        using reference = iunordered_set::const_reference; //!< Reference alias.
        using pointer = iunordered_set::const_pointer; //!< Pointer alias.
        using iterator_category = bidirectional_iterator_tag; //!< Iterator category alias.

        /**
         * @brief Default constructor.
         */
        const_iterator() = default;

        /**
         * @brief Constructor.
         * @param it Non const iterator.
         */
        const_iterator(const iterator& it) :
            _index(it._index),
            _set(it._set)
        {
        }

        /**
         * @brief Increments the position.
         * @return Reference to this.
         */
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

        /**
         * @brief Increments the position.
         * @return The iterator before being incremented.
         */
        const_iterator operator++(int)
        {
            const_iterator copy(*this);
            operator++();
            return copy;
        }

        /**
         * @brief Decrements the position.
         * @return Reference to this.
         */
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

        /**
         * @brief Decrements the position.
         * @return The iterator before being decremented.
         */
        const_iterator operator--(int)
        {
            const_iterator copy(*this);
            operator--();
            return copy;
        }

        /**
         * @brief Returns a const reference to the pointed value.
         */
        [[nodiscard]] const_reference operator*() const
        {
            return _set->_storage[_index];
        }

        /**
         * @brief Returns a const pointer to the pointed value.
         */
        const_pointer operator->() const
        {
            return _set->_storage + _index;
        }

        /**
         * @brief Equal operator.
         * @param a First const_iterator to compare.
         * @param b Second const_iterator to compare.
         * @return `true` if the first const_iterator is equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator==(const const_iterator& a, const const_iterator& b)
        {
            return a._index == b._index;
        }

        /**
         * @brief Not equal operator.
         * @param a First const_iterator to compare.
         * @param b Second const_iterator to compare.
         * @return `true` if the first const_iterator is not equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator!=(const const_iterator& a, const const_iterator& b)
        {
            return a._index != b._index;
        }

    private:
        friend class iunordered_set;
        friend class iterator;

        size_type _index = 0;
        const iunordered_set* _set = nullptr;

        const_iterator(size_type index, const iunordered_set& set) :
            _index(index),
            _set(&set)
        {
        }
    };

    using reverse_iterator = bn::reverse_iterator<iterator>; //!< Reverse iterator alias.
    using const_reverse_iterator = bn::reverse_iterator<const_iterator>; //!< Const reverse iterator alias.

    iunordered_set(const iunordered_set& other) = delete;

    /**
     * @brief Destructor.
     */
    ~iunordered_set() noexcept = default;

    /**
     * @brief Destructor.
     */
    ~iunordered_set() noexcept
    requires(! is_trivially_destructible_v<value_type>)
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
        }
    }

    /**
     * @brief Copy assignment operator.
     * @param other iunordered_set to copy.
     * @return Reference to this.
     */
    iunordered_set& operator=(const iunordered_set& other)
    {
        if(this != &other)
        {
            BN_ASSERT(other._size <= max_size(), "Not enough space: ", max_size(), " - ", other._size);

            clear();
            _assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param other iunordered_set to move.
     * @return Reference to this.
     */
    iunordered_set& operator=(iunordered_set&& other) noexcept
    {
        if(this != &other)
        {
            BN_ASSERT(other._size <= max_size(), "Not enough space: ", max_size(), " - ", other._size);

            clear();
            _assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Returns the current size.
     */
    [[nodiscard]] size_type size() const
    {
        return _size;
    }

    /**
     * @brief Returns the maximum possible size.
     */
    [[nodiscard]] size_type max_size() const
    {
        return _max_size_minus_one + 1;
    }

    /**
     * @brief Returns the remaining capacity.
     */
    [[nodiscard]] size_type available() const
    {
        return max_size() - _size;
    }

    /**
     * @brief Indicates if it doesn't contain any element.
     */
    [[nodiscard]] bool empty() const
    {
        return _size == 0;
    }

    /**
     * @brief Indicates if it can't contain any more elements.
     */
    [[nodiscard]] bool full() const
    {
        return _size == max_size();
    }

    /**
     * @brief Returns a const iterator to the beginning of the iunordered_set.
     */
    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(_first_valid_index, *this);
    }

    /**
     * @brief Returns an iterator to the beginning of the iunordered_set.
     */
    [[nodiscard]] iterator begin()
    {
        return iterator(_first_valid_index, *this);
    }

    /**
     * @brief Returns a const iterator to the end of the iunordered_set.
     */
    [[nodiscard]] const_iterator end() const
    {
        return const_iterator(max_size(), *this);
    }

    /**
     * @brief Returns an iterator to the end of the iunordered_set.
     */
    [[nodiscard]] iterator end()
    {
        return iterator(max_size(), *this);
    }

    /**
     * @brief Returns a const iterator to the beginning of the iunordered_set.
     */
    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(_first_valid_index, *this);
    }

    /**
     * @brief Returns a const iterator to the end of the iunordered_set.
     */
    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator(max_size(), *this);
    }

    /**
     * @brief Returns a const reverse iterator to the end of the iunordered_set.
     */
    [[nodiscard]] const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator to the end of the iunordered_set.
     */
    [[nodiscard]] reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the iunordered_set.
     */
    [[nodiscard]] const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a reverse iterator to the beginning of the iunordered_set.
     */
    [[nodiscard]] reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns a const reverse iterator to the end of the iunordered_set.
     */
    [[nodiscard]] const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the iunordered_set.
     */
    [[nodiscard]] const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    /**
     * @brief Indicates if the specified key is contained in this iunordered_set.
     * @param key Key to search for.
     * @return `true` if the specified key is contained in this iunordered_set, otherwise `false`.
     */
    [[nodiscard]] bool contains(const key_type& key) const
    {
        if(empty())
        {
            return false;
        }

        return contains_hash(hasher()(key), key);
    }

    /**
     * @brief Indicates if the specified key is contained in this iunordered_set.
     * @param key_hash Hash of the given key to search for.
     * @param key Key to search for.
     * @return `true` if the specified key is contained in this iunordered_set, otherwise `false`.
     */
    [[nodiscard]] bool contains_hash(hash_type key_hash, const key_type& key) const
    {
        return find_hash(key_hash, key) != end();
    }

    /**
     * @brief Counts the number of keys stored in this iunordered_set are equal to the given one.
     * @param key Key to search for.
     * @return 1 if the specified key is contained in this iunordered_set, otherwise 0.
     */
    [[nodiscard]] size_type count(const key_type& key) const
    {
        return count_hash(hasher()(key), key);
    }

    /**
     * @brief Counts the number of keys stored in this iunordered_set are equal to the given one.
     * @param key_hash Hash of the given key to search for.
     * @param key Key to search for.
     * @return 1 if the specified key is contained in this iunordered_set, otherwise 0.
     */
    [[nodiscard]] size_type count_hash(hash_type key_hash, const key_type& key) const
    {
        return contains_hash(key_hash, key) ? 1 : 0;
    }

    /**
     * @brief Searches for a given key.
     * @param key Key to search for.
     * @return Const iterator to the key if it exists, otherwise end().
     */
    [[nodiscard]] const_iterator find(const key_type& key) const
    {
        return const_cast<iunordered_set&>(*this).find(key);
    }

    /**
     * @brief Searches for a given key.
     * @param key Key to search for.
     * @return Iterator to the key if it exists, otherwise end().
     */
    [[nodiscard]] iterator find(const key_type& key)
    {
        if(empty())
        {
            return end();
        }

        return find_hash(hasher()(key), key);
    }

    /**
     * @brief Searches for a given key.
     * @param key_hash Hash of the given key to search for.
     * @param key Key to search for.
     * @return Const iterator to the key if it exists, otherwise end().
     */
    [[nodiscard]] const_iterator find_hash(hash_type key_hash, const key_type& key) const
    {
        return const_cast<iunordered_set&>(*this).find_hash(key_hash, key);
    }

    /**
     * @brief Searches for a given key.
     * @param key_hash Hash of the given key to search for.
     * @param key Key to search for.
     * @return Iterator to the key if it exists, otherwise end().
     */
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

    /**
     * @brief Inserts a copy of the given value.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(const value_type& value)
    {
        return insert_hash(hasher()(value), value);
    }

    /**
     * @brief Inserts a moved value.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(value_type&& value)
    {
        return insert_hash(hasher()(value), move(value));
    }

    /**
     * @brief Inserts a copy of the given value.
     * @param value_hash Hash of the value to insert.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert_hash(hash_type value_hash, const value_type& value)
    {
        return insert_hash(value_hash, value_type(value));
    }

    /**
     * @brief Inserts a moved value.
     * @param value_hash Hash of the value to insert.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert_hash(hash_type value_hash, value_type&& value)
    {
        size_type index = _index(value_hash);
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
            BN_BASIC_ASSERT(current_index != index, "All indices are allocated");
        }

        ::new(static_cast<void*>(storage + current_index)) value_type(move(value));
        allocated[current_index] = true;
        _first_valid_index = min(_first_valid_index, current_index);
        _last_valid_index = max(_last_valid_index, current_index);
        ++_size;
        return iterator(current_index, *this);
    }

    /**
     * @brief Erases an element.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param position Iterator to the element to erase.
     * @return Iterator following the erased element.
     */
    iterator erase(const const_iterator& position)
    {
        bool* allocated = _allocated;
        size_type index = position._index;
        BN_BASIC_ASSERT(allocated[index], "Index is not allocated: ", index);

        pointer storage = _storage;
        storage[index].~value_type();
        allocated[index] = false;
        --_size;

        if(! _size)
        {
            _first_valid_index = max_size();
            _last_valid_index = 0;
            return end();
        }

        size_type first_valid_index = _first_valid_index;

        if(index == first_valid_index)
        {
            while(! allocated[first_valid_index])
            {
                ++first_valid_index;
            }

            _first_valid_index = first_valid_index;
        }

        size_type last_valid_index = _last_valid_index;

        if(index == last_valid_index)
        {
            while(! allocated[last_valid_index])
            {
                --last_valid_index;
            }

            _last_valid_index = last_valid_index;
        }

        size_type next_index = _index(index + 1);
        int reinsert_count = 0;

        while(allocated[next_index])
        {
            ++reinsert_count;
            next_index = _index(next_index + 1);
        }

        next_index = _index(index + 1);

        for(int reinsert_index = 0; reinsert_index < reinsert_count; ++reinsert_index)
        {
            value_type temp_value(move(storage[next_index]));
            storage[next_index].~value_type();
            allocated[next_index] = false;
            --_size;

            if(_size)
            {
                first_valid_index = _first_valid_index;

                if(next_index == first_valid_index)
                {
                    while(! allocated[first_valid_index])
                    {
                        ++first_valid_index;
                    }

                    _first_valid_index = first_valid_index;
                }

                last_valid_index = _last_valid_index;

                if(next_index == last_valid_index)
                {
                    while(! allocated[last_valid_index])
                    {
                        --last_valid_index;
                    }

                    _last_valid_index = last_valid_index;
                }
            }
            else
            {
                _first_valid_index = max_size();
                _last_valid_index = 0;
            }

            insert(move(temp_value));
            next_index = _index(next_index + 1);
        }

        last_valid_index = _last_valid_index;

        while(index <= last_valid_index)
        {
            if(allocated[index])
            {
                return iterator(index, *this);
            }

            ++index;
        }

        return end();
    }

    /**
     * @brief Erases an element.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param key Key to erase.
     * @return `true` if the elements was erased, otherwise `false`.
     */
    bool erase(const key_type& key)
    {
        return erase_hash(hasher()(key), key);
    }

    /**
     * @brief Erases an element.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param key_hash Hash of the key to erase.
     * @param key Key to erase.
     * @return `true` if the elements was erased, otherwise `false`.
     */
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

    /**
     * @brief Erases all elements that satisfy the specified predicate.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param pred Unary predicate which returns ​true if the element should be erased.
     * @return Number of erased elements.
     */
    template<class Pred>
    size_type erase_if(const Pred& pred)
    {
        size_type erased_count = 0;
        pointer storage = _storage;
        bool* allocated = _allocated;
        size_type first_valid_index = max_size();
        size_type last_valid_index = 0;

        for(size_type index = _first_valid_index, last = _last_valid_index; index <= last; ++index)
        {
            if(allocated[index])
            {
                if(allocated[index] && pred(storage[index]))
                {
                    storage[index].~value_type();
                    allocated[index] = false;
                    ++erased_count;
                }
                else
                {
                    first_valid_index = min(index, first_valid_index);
                    last_valid_index = max(index, last_valid_index);
                }
            }
        }

        _size -= erased_count;
        _first_valid_index = first_valid_index;
        _last_valid_index = last_valid_index;
        return erased_count;
    }

    /**
     * @brief Moves all elements of the given iunordered_set into this one, leaving the first one empty.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     */
    void merge(iunordered_set&& other)
    {
        if(this != &other)
        {
            BN_ASSERT(_max_size_minus_one == other._max_size_minus_one,
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
                    if(allocated[index])
                    {
                        storage[index] = move(other_storage[index]);
                    }
                    else
                    {
                        ::new(static_cast<void*>(storage + index)) value_type(move(other_storage[index]));
                        ++size;
                    }
                }
            }

            _size = size;
            _first_valid_index = first_valid_index;
            _last_valid_index = last_valid_index;
            other.clear();
        }
    }

    /**
     * @brief Removes all elements.
     */
    void clear()
    {
        if(_size)
        {
            size_type first_valid_index = _first_valid_index;
            memory::clear(_last_valid_index - first_valid_index + 1, _allocated[first_valid_index]);
            _first_valid_index = _max_size_minus_one + 1;
            _last_valid_index = 0;
            _size = 0;
        }
    }

    /**
     * @brief Removes all elements.
     */
    void clear()
    requires(! is_trivially_destructible_v<value_type>)
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
                    allocated[index] = false;
                    storage[index].~value_type();
                }
            }

            _first_valid_index = _max_size_minus_one + 1;
            _last_valid_index = 0;
            _size = 0;
        }
    }

    /**
     * @brief Exchanges the contents of this iunordered_set with those of the other one.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param other iunordered_set to exchange the contents with.
     */
    void swap(iunordered_set& other)
    {
        if(this != &other)
        {
            BN_ASSERT(_max_size_minus_one == other._max_size_minus_one,
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
                        bn::swap(storage[index], other_storage[index]);
                    }
                    else
                    {
                        ::new(static_cast<void*>(storage + index)) value_type(move(other_storage[index]));
                        other_storage[index].~value_type();
                        other_allocated[index] = false;
                        allocated[index] = true;
                    }
                }
                else
                {
                    if(allocated[index])
                    {
                        ::new(static_cast<void*>(other_storage + index)) value_type(move(storage[index]));
                        storage[index].~value_type();
                        allocated[index] = false;
                        other_allocated[index] = true;
                    }
                }
            }

            bn::swap(_size, other._size);
            bn::swap(_first_valid_index, other._first_valid_index);
            bn::swap(_last_valid_index, other._last_valid_index);
        }
    }

    /**
     * @brief Exchanges the contents of a iunordered_set with those of another one.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param a First iunordered_set to exchange the contents with.
     * @param b Second iunordered_set to exchange the contents with.
     */
    friend void swap(iunordered_set& a, iunordered_set& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First iunordered_set to compare.
     * @param b Second iunordered_set to compare.
     * @return `true` if the first iunordered_set is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const iunordered_set& a, const iunordered_set& b)
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

    /**
     * @brief Less than operator.
     * @param a First iunordered_set to compare.
     * @param b Second iunordered_set to compare.
     * @return `true` if the first iunordered_set is lexicographically less than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<(const iunordered_set& a, const iunordered_set& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First iunordered_set to compare.
     * @param b Second iunordered_set to compare.
     * @return `true` if the first iunordered_set is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>(const iunordered_set& a, const iunordered_set& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First iunordered_set to compare.
     * @param b Second iunordered_set to compare.
     * @return `true` if the first iunordered_set is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<=(const iunordered_set& a, const iunordered_set& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First iunordered_set to compare.
     * @param b Second iunordered_set to compare.
     * @return `true` if the first iunordered_set is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>=(const iunordered_set& a, const iunordered_set& b)
    {
        return ! (a < b);
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    iunordered_set(reference storage, bool& allocated, size_type max_size) :
        _storage(&storage),
        _allocated(&allocated),
        _max_size_minus_one(max_size - 1),
        _first_valid_index(max_size)
    {
    }

    void _assign(const iunordered_set& other)
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
                ::new(static_cast<void*>(storage + index)) value_type(other_storage[index]);
            }
        }

        _first_valid_index = other._first_valid_index;
        _last_valid_index = other._last_valid_index;
        _size = other._size;
    }

    void _assign(iunordered_set&& other)
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
                ::new(static_cast<void*>(storage + index)) value_type(move(other_storage[index]));
            }
        }

        _first_valid_index = other._first_valid_index;
        _last_valid_index = other._last_valid_index;
        _size = other._size;
        other.clear();
    }

    /// @endcond

private:
    pointer _storage;
    bool* _allocated;
    size_type _max_size_minus_one;
    size_type _first_valid_index;
    size_type _last_valid_index = 0;
    size_type _size = 0;

    [[nodiscard]] size_type _index(hash_type key_hash) const
    {
        return key_hash & _max_size_minus_one;
    }
};


template<typename Key, int MaxSize, typename KeyHash, typename KeyEqual>
class unordered_set : public iunordered_set<Key, KeyHash, KeyEqual>
{
    static_assert(power_of_two(MaxSize));

public:
    using key_type = Key; //!< Key type alias.
    using value_type = Key; //!< Value type alias.
    using size_type = int; //!< Size type alias.
    using difference_type = int; //!< Difference type alias.
    using hash_type = unsigned; //!< Hash type alias.
    using hasher = KeyHash; //!< Hash functor alias.
    using key_equal = KeyEqual; //!< Equality functor alias.
    using reference = value_type&; //!< Reference alias.
    using const_reference = const value_type&; //!< Const reference alias.
    using pointer = value_type*; //!< Pointer alias.
    using const_pointer = const value_type*; //!< Const pointer alias.

    /**
     * @brief Default constructor.
     */
    unordered_set() :
        iunordered_set<Key, KeyHash, KeyEqual>(*reinterpret_cast<pointer>(_storage_buffer), *_allocated_buffer, MaxSize)
    {
    }

    /**
     * @brief Copy constructor.
     * @param other unordered_set to copy.
     */
    unordered_set(const unordered_set& other) :
        unordered_set()
    {
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param other unordered_set to move.
     */
    unordered_set(unordered_set&& other) noexcept :
        unordered_set()
    {
        this->_assign(move(other));
    }

    /**
     * @brief Copy constructor.
     * @param other iunordered_set to copy.
     */
    unordered_set(const iunordered_set<Key, KeyHash, KeyEqual>& other) :
        unordered_set()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param other iunordered_set to move.
     */
    unordered_set(iunordered_set<Key, KeyHash, KeyEqual>&& other) noexcept :
        unordered_set()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(move(other));
    }

    /**
     * @brief Copy assignment operator.
     * @param other unordered_set to copy.
     * @return Reference to this.
     */
    unordered_set& operator=(const unordered_set& other)
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param other unordered_set to move.
     * @return Reference to this.
     */
    unordered_set& operator=(unordered_set&& other) noexcept
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other iunordered_set to copy.
     * @return Reference to this.
     */
    unordered_set& operator=(const iunordered_set<Key, KeyHash, KeyEqual>& other)
    {
        if(this != &other)
        {
            BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * Unlike `std::unordered_set`, it doesn't offer pointer stability.
     *
     * @param other iunordered_set to move.
     * @return Reference to this.
     */
    unordered_set& operator=(iunordered_set<Key, KeyHash, KeyEqual>&& other) noexcept
    {
        if(this != &other)
        {
            BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

private:
    static constexpr unsigned _alignment = alignof(value_type) > alignof(int) ? alignof(value_type) : alignof(int);

    alignas(_alignment) char _storage_buffer[sizeof(value_type) * MaxSize];
    bool _allocated_buffer[MaxSize] = {};
};


/**
 * @brief Erases all elements from a iunordered_set that satisfy the specified predicate.
 *
 * Unlike `std::unordered_set`, it doesn't offer pointer stability.
 *
 * @param set iunordered_set from which to erase.
 * @param pred Unary predicate which returns ​true if the element should be erased.
 * @return Number of erased elements.
 */
template<typename Key, typename KeyHash, typename KeyEqual, class Pred>
typename iunordered_set<Key, KeyHash, KeyEqual>::size_type erase_if(
        iunordered_set<Key, KeyHash, KeyEqual>& set, const Pred& pred)
{
    return set.erase_if(pred);
}

}

#endif
