#ifndef BTN_MEMORY_H
#define BTN_MEMORY_H

#include "btn_assert.h"
#include "btn_utility.h"
#include "btn_functional.h"

namespace btn
{
    template<typename Type>
    struct default_delete
    {
        void operator()(Type* ptr) const
        {
            delete ptr;
        }
    };


    template<typename Type>
    struct default_delete<Type[]>
    {
        template<class U>
        void operator()(U* ptr) const
        {
            delete[] ptr;
        }
    };


    template<typename Type, typename Deleter = default_delete<Type>>
    class unique_ptr
    {

    public:
        using pointer = Type*;
        using reference = Type&;
        using element_type = Type;
        using deleter_type = Deleter;

        unique_ptr() = default;

        explicit unique_ptr(pointer ptr) :
            _ptr(ptr)
        {
        }

        unique_ptr(const unique_ptr& other) = delete;

        unique_ptr(unique_ptr&& other) :
            _ptr(other.release()),
            _deleter(move(other._deleter))
        {
        }

        unique_ptr& operator=(const unique_ptr& other) = delete;

        unique_ptr& operator=(unique_ptr&& other)
        {
            reset(other.release());
            _deleter = move(other._deleter);
            return *this;
        }

        ~unique_ptr()
        {
            reset();
        }

        [[nodiscard]] pointer get() const
        {
            return _ptr;
        }

        [[nodiscard]] const Deleter& get_deleter() const
        {
            return _deleter;
        }

        [[nodiscard]] Deleter& get_deleter()
        {
            return _deleter;
        }

        pointer release()
        {
            pointer result = _ptr;
            _ptr = nullptr;
            return result;
        }

        void reset()
        {
            _deleter(_ptr);
            _ptr = nullptr;
        }

        void reset(pointer ptr)
        {
            if(ptr != _ptr)
            {
                reset();
                _ptr = ptr;
            }
        }

        void swap(unique_ptr& other)
        {
            swap(_ptr, other._ptr);
            swap(_deleter, other._deleter);
        }

        [[nodiscard]] operator bool() const
        {
            return _ptr != nullptr;
        }

        [[nodiscard]] reference operator*() const
        {
            BTN_ASSERT(_ptr, "Managed pointer is null");

            return *_ptr;
        }

        [[nodiscard]] pointer operator->() const
        {
            BTN_ASSERT(_ptr, "Managed pointer is null");

            return _ptr;
        }

        [[nodiscard]] bool operator==(const unique_ptr& other) const
        {
            return _ptr == other._ptr;
        }

        [[nodiscard]] bool operator!=(const unique_ptr& other) const
        {
            return _ptr != other._ptr;
        }

        [[nodiscard]] bool operator<(const unique_ptr& other) const
        {
            return _ptr < other._ptr;
        }

        [[nodiscard]] bool operator<=(const unique_ptr& other) const
        {
            return _ptr <= other._ptr;
        }

        [[nodiscard]] bool operator>(const unique_ptr& other) const
        {
            return _ptr > other._ptr;
        }

        [[nodiscard]] bool operator>=(const unique_ptr& other) const
        {
            return _ptr >= other._ptr;
        }

    private:
        pointer _ptr = nullptr;
        deleter_type _deleter;
    };


    template<typename Type, typename Deleter>
    class unique_ptr<Type[], Deleter>
    {

    public:
        using pointer = Type*;
        using reference = Type&;
        using element_type = Type;
        using deleter_type = Deleter;

        unique_ptr() = default;

        explicit unique_ptr(pointer ptr) :
            _ptr(ptr)
        {
        }

        unique_ptr(const unique_ptr& other) = delete;

        unique_ptr(unique_ptr&& other) :
            _ptr(other.release()),
            _deleter(move(other._deleter))
        {
        }

        unique_ptr& operator=(const unique_ptr& other) = delete;

        unique_ptr& operator=(unique_ptr&& other)
        {
            reset(other.release());
            _deleter = move(other._deleter);
            return *this;
        }

        ~unique_ptr()
        {
            reset();
        }

        [[nodiscard]] pointer get() const
        {
            return _ptr;
        }

        [[nodiscard]] const Deleter& get_deleter() const
        {
            return _deleter;
        }

        [[nodiscard]] Deleter& get_deleter()
        {
            return _deleter;
        }

        [[nodiscard]] pointer release()
        {
            pointer result = _ptr;
            _ptr = nullptr;
            return result;
        }

        void reset()
        {
            _deleter(_ptr);
            _ptr = nullptr;
        }

        void reset(pointer ptr)
        {
            if(ptr != _ptr)
            {
                reset();
                _ptr = ptr;
            }
        }

        void swap(unique_ptr& other)
        {
            swap(_ptr, other._ptr);
            swap(_deleter, other._deleter);
        }

        [[nodiscard]] operator bool() const
        {
            return _ptr != nullptr;
        }

        [[nodiscard]] reference operator*() const
        {
            BTN_ASSERT(_ptr, "Managed pointer is null");

            return *_ptr;
        }

        [[nodiscard]] pointer operator->() const
        {
            BTN_ASSERT(_ptr, "Managed pointer is null");

            return _ptr;
        }

        [[nodiscard]] bool operator==(const unique_ptr& other) const
        {
            return _ptr == other._ptr;
        }

        [[nodiscard]] bool operator!=(const unique_ptr& other) const
        {
            return _ptr != other._ptr;
        }

        [[nodiscard]] bool operator<(const unique_ptr& other) const
        {
            return _ptr < other._ptr;
        }

        [[nodiscard]] bool operator<=(const unique_ptr& other) const
        {
            return _ptr <= other._ptr;
        }

        [[nodiscard]] bool operator>(const unique_ptr& other) const
        {
            return _ptr > other._ptr;
        }

        [[nodiscard]] bool operator>=(const unique_ptr& other) const
        {
            return _ptr >= other._ptr;
        }

    private:
        pointer _ptr = nullptr;
        deleter_type _deleter;
    };


    template<typename Type, class... Args>
    [[nodiscard]] unique_ptr<Type> make_unique(Args&&... args)
    {
        return unique_ptr<Type>(new Type(forward<Args>(args)...));
    }


    template<typename Type, typename Deleter>
    struct hash<unique_ptr<Type, Deleter>>
    {
        [[nodiscard]] size_t operator()(const unique_ptr<Type, Deleter>& value) const
        {
            return make_hash(value.get());
        }
    };


    [[nodiscard]] int used_static_iwram();

    [[nodiscard]] int used_static_ewram();

    [[nodiscard]] int used_malloc_ewram();
}

#endif
