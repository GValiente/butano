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

        unique_ptr(unique_ptr&& other) noexcept :
            _ptr(other.release()),
            _deleter(move(other._deleter))
        {
        }

        unique_ptr& operator=(const unique_ptr& other) = delete;

        unique_ptr& operator=(unique_ptr&& other) noexcept
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

        void swap(unique_ptr& other)
        {
            btn::swap(_ptr, other._ptr);
            btn::swap(_deleter, other._deleter);
        }

        friend void swap(unique_ptr& a, unique_ptr& b)
        {
            a.swap(b);
        }

        [[nodiscard]] friend bool operator==(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr == b._ptr;
        }

        [[nodiscard]] friend bool operator!=(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr != b._ptr;
        }

        [[nodiscard]] friend bool operator<(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr < b._ptr;
        }

        [[nodiscard]] friend bool operator<=(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr <= b._ptr;
        }

        [[nodiscard]] friend bool operator>(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr > b._ptr;
        }

        [[nodiscard]] friend bool operator>=(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr >= b._ptr;
        }

        [[nodiscard]] friend bool operator==(const unique_ptr& a, decltype(nullptr))
        {
            return ! a._ptr;
        }

        [[nodiscard]] friend bool operator!=(const unique_ptr& a, decltype(nullptr))
        {
            return a._ptr;
        }

        [[nodiscard]] friend bool operator<(const unique_ptr&, decltype(nullptr))
        {
            return false;
        }

        [[nodiscard]] friend bool operator<=(const unique_ptr& a, decltype(nullptr))
        {
            return ! a._ptr;
        }

        [[nodiscard]] friend bool operator>(const unique_ptr& a, decltype(nullptr))
        {
            return a._ptr;
        }

        [[nodiscard]] friend bool operator>=(const unique_ptr&, decltype(nullptr))
        {
            return true;
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

        unique_ptr(unique_ptr&& other) noexcept :
            _ptr(other.release()),
            _deleter(move(other._deleter))
        {
        }

        unique_ptr& operator=(const unique_ptr& other) = delete;

        unique_ptr& operator=(unique_ptr&& other) noexcept
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

        [[nodiscard]] friend bool operator==(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr == b._ptr;
        }

        [[nodiscard]] friend bool operator!=(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr != b._ptr;
        }

        [[nodiscard]] friend bool operator<(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr < b._ptr;
        }

        [[nodiscard]] friend bool operator<=(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr <= b._ptr;
        }

        [[nodiscard]] friend bool operator>(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr > b._ptr;
        }

        [[nodiscard]] friend bool operator>=(const unique_ptr& a, const unique_ptr& b)
        {
            return a._ptr >= b._ptr;
        }

        [[nodiscard]] friend bool operator==(const unique_ptr& a, decltype(nullptr))
        {
            return ! a._ptr;
        }

        [[nodiscard]] friend bool operator!=(const unique_ptr& a, decltype(nullptr))
        {
            return a._ptr;
        }

        [[nodiscard]] friend bool operator<(const unique_ptr&, decltype(nullptr))
        {
            return false;
        }

        [[nodiscard]] friend bool operator<=(const unique_ptr& a, decltype(nullptr))
        {
            return ! a._ptr;
        }

        [[nodiscard]] friend bool operator>(const unique_ptr& a, decltype(nullptr))
        {
            return a._ptr;
        }

        [[nodiscard]] friend bool operator>=(const unique_ptr&, decltype(nullptr))
        {
            return true;
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
        [[nodiscard]] unsigned operator()(const unique_ptr<Type, Deleter>& value) const
        {
            return make_hash(value.get());
        }
    };
}


namespace _btn::memory
{
    void unsafe_copy(const void* source, int bytes, void* destination);

    void unsafe_copy16(const void* source, int half_words, void* destination);

    void unsafe_copy32(const void* source, int words, void* destination);

    void unsafe_clear(int bytes, void* destination);

    void unsafe_clear16(int half_words, void* destination);

    void unsafe_clear32(int words, void* destination);
}


namespace btn::memory
{
    [[nodiscard]] void* ewram_alloc(int bytes);

    void ewram_free(void* ptr);

    [[nodiscard]] int used_alloc_ewram();

    [[nodiscard]] int available_alloc_ewram();

    [[nodiscard]] int used_static_iwram();

    [[nodiscard]] int used_static_ewram();

    template<typename Type>
    void copy(const Type& source, int elements, Type& destination)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        BTN_ASSERT(elements >= 0, "Invalid elements: ", elements);

        int bytes = elements * int(sizeof(Type));

        if(sizeof(Type) % 4 == 0 && alignof(Type) % 4 == 0)
        {
            _btn::memory::unsafe_copy32(&source, bytes / 4, &destination);
        }
        else if(sizeof(Type) % 2 == 0 && alignof(Type) % 2 == 0)
        {
            _btn::memory::unsafe_copy16(&source, bytes / 2, &destination);
        }
        else
        {
            _btn::memory::unsafe_copy(&source, bytes, &destination);
        }
    }

    template<typename Type>
    void clear(int elements, Type& destination)
    {
        static_assert(is_trivial<Type>(), "Type is not trivial");
        BTN_ASSERT(elements >= 0, "Invalid elements: ", elements);

        int bytes = elements * int(sizeof(Type));

        if(sizeof(Type) % 4 == 0 && alignof(Type) % 4 == 0)
        {
            _btn::memory::unsafe_clear32(bytes / 4, &destination);
        }
        else if(sizeof(Type) % 2 == 0 && alignof(Type) % 2 == 0)
        {
            _btn::memory::unsafe_clear16(bytes / 2, &destination);
        }
        else
        {
            _btn::memory::unsafe_clear(bytes, &destination);
        }
    }

    void set(uint8_t value, int bytes, void* destination);

    void set16(uint16_t value, int half_words, void* destination);

    void set32(unsigned value, int words, void* destination);
}

#endif
