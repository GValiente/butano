#ifndef BTN_ANY_H
#define BTN_ANY_H

#include <new>
#include "btn_assert.h"
#include "btn_type_id.h"
#include "btn_utility.h"
#include "btn_any_fwd.h"

namespace btn
{

class iany
{

public:
    iany(const iany& other) = delete;

    iany& operator=(const iany& other)
    {
        if(this != &other)
        {
            reset();
            _assign(other);
        }

        return *this;
    }

    iany& operator=(iany&& other)
    {
        if(this != &other)
        {
            reset();
            _assign(move(other));
        }

        return *this;
    }

    template<typename Type>
    iany& operator=(const Type& value)
    {
        BTN_ASSERT(int(sizeof(Type)) <= _max_size, "Invalid value size: ", sizeof(Type), " - ", _max_size);
        BTN_ASSERT(int(alignof(Type)) <= _max_alignment, "Invalid value alignment: ",
                   alignof(Type), " - ", _max_alignment);

        reset();
        ::new(_value_ptr<Type>()) Type(value);
        _create_manager<Type>();
        return *this;
    }

    template<typename Type>
    iany& operator=(Type&& value)
    {
        BTN_ASSERT(int(sizeof(Type)) <= _max_size, "Invalid value size: ", sizeof(Type), " - ", _max_size);
        BTN_ASSERT(int(alignof(Type)) <= _max_alignment, "Invalid value alignment: ",
                   alignof(Type), " - ", _max_alignment);

        reset();
        ::new(_value_ptr<Type>()) Type(move(value));
        _create_manager<Type>();
        return *this;
    }

    ~iany()
    {
        reset();
    }

    [[nodiscard]] explicit operator bool() const
    {
        return _manager_created;
    }

    [[nodiscard]] bool has_value() const
    {
        return _manager_created;
    }

    [[nodiscard]] type_id_t type() const
    {
        return has_value() ? _manager_ptr()->type() : type_id_t();
    }

    [[nodiscard]] int max_size() const
    {
        return _max_size;
    }

    [[nodiscard]] int max_alignment() const
    {
        return _max_alignment;
    }

    template<typename Type>
    [[nodiscard]] const Type& value() const
    {
        BTN_ASSERT(type() == type_id<Type>(), "Invalid value type");

        return *_value_ptr<Type>();
    }

    template<typename Type>
    [[nodiscard]] Type& value()
    {
        BTN_ASSERT(type() == type_id<Type>(), "Invalid value type");

        return *_value_ptr<Type>();
    }

    template<typename Type>
    [[nodiscard]] Type value_or(const Type& default_value) const
    {
        return has_value() ? value<Type>() : default_value;
    }

    template<typename Type>
    [[nodiscard]] Type value_or(Type&& default_value) const
    {
        return has_value() ? value<Type>() : move(default_value);
    }

    template<typename Type, typename... Args>
    void emplace(Args&&... args)
    {
        BTN_ASSERT(int(sizeof(Type)) <= _max_size, "Invalid value size: ", sizeof(Type), " - ", _max_size);
        BTN_ASSERT(int(alignof(Type)) <= _max_alignment, "Invalid value alignment: ",
                   alignof(Type), " - ", _max_alignment);

        reset();
        ::new(_value_ptr<Type>()) Type(forward<Args>(args)...);
        _create_manager<Type>();
    }

    void reset()
    {
        if(_manager_created)
        {
            base_manager* manager = _manager_ptr();
            manager->destroy(*this);
            manager->~base_manager();
            _manager_created = false;
        }
    }

    void swap(iany& other)
    {
        if(has_value())
        {
            if(other.has_value())
            {
                base_manager* manager = _manager_ptr();
                BTN_ASSERT(manager->type() == other._manager_ptr()->type(), "Value type mismatch");

                manager->swap(*this, other);
            }
            else
            {
                other._assign(move(*this));
            }
        }
        else
        {
            _assign(move(other));
        }
    }

    friend void swap(iany& a, iany& b)
    {
        a.swap(b);
    }

protected:
    class base_manager
    {

    public:
        virtual ~base_manager() = default;

        [[nodiscard]] virtual type_id_t type() const = 0;

        virtual void copy_to(const iany& this_any, iany& other_any) const = 0;

        virtual void move_to(iany& this_any, iany& other_any) const = 0;

        virtual void swap(iany& this_any, iany& other_any) const = 0;

        virtual void destroy(iany& any) const = 0;
    };

    template<typename Type>
    class type_manager : public base_manager
    {

    public:
        [[nodiscard]] type_id_t type() const override final
        {
            return type_id<Type>();
        }

        void copy_to(const iany& this_any, iany& other_any) const override final
        {
            BTN_ASSERT(int(sizeof(Type)) <= other_any.max_size(), "Invalid value size: ",
                       sizeof(Type), " - ", other_any.max_size());
            BTN_ASSERT(int(alignof(Type)) <= other_any.max_alignment(), "Invalid value alignment: ",
                       alignof(Type), " - ", other_any.max_alignment());

            if(is_copy_constructible_v<Type>)
            {
                other_any.reset();
                ::new(other_any._value_ptr<Type>()) Type(*this_any._value_ptr<Type>());
                other_any._create_manager<Type>();
            }
            else
            {
                BTN_ERROR("This type can't be copied");
            }
        }

        void move_to(iany& this_any, iany& other_any) const override final
        {
            BTN_ASSERT(int(sizeof(Type)) <= other_any.max_size(), "Invalid value size: ",
                       sizeof(Type), " - ", other_any.max_size());
            BTN_ASSERT(int(alignof(Type)) <= other_any.max_alignment(), "Invalid value alignment: ",
                       alignof(Type), " - ", other_any.max_alignment());

            if(is_move_constructible_v<Type>)
            {
                other_any.reset();
                ::new(other_any._value_ptr<Type>()) Type(move(*this_any._value_ptr<Type>()));
                other_any._create_manager<Type>();
                this_any.reset();
            }
            else
            {
                BTN_ERROR("This type can't be moved");
            }
        }

        void swap(iany& this_any, iany& other_any) const override final
        {
            if(is_swappable_v<Type>)
            {
                btn::swap(*this_any._value_ptr<Type>(), *other_any._value_ptr<Type>());
            }
            else
            {
                BTN_ERROR("This type can't be swapped");
            }
        }

        void destroy(iany& any) const override final
        {
            any._value_ptr<Type>()->~Type();
        }
    };

    iany(char* storage, int max_size, int max_alignment) :
        _storage(storage),
        _max_size(max_size),
        _max_alignment(int16_t(max_alignment)),
        _manager_created(false)
    {
    }

    template<typename Type>
    [[nodiscard]] const Type* _value_ptr() const
    {
        return reinterpret_cast<const Type*>(_storage);
    }

    template<typename Type>
    [[nodiscard]] Type* _value_ptr()
    {
        return reinterpret_cast<Type*>(_storage);
    }

    template<typename Type>
    void _create_manager()
    {
        static_assert(sizeof(type_manager<Type>) == sizeof(base_manager));
        static_assert(alignof(type_manager<Type>) == alignof(base_manager));

        ::new(_manager_ptr()) type_manager<Type>();
        _manager_created = true;
    }

    void _assign(const iany& other)
    {
        if(other.has_value())
        {
            other._manager_ptr()->copy_to(other, *this);
        }
    }

    void _assign(iany&& other)
    {
        if(other.has_value())
        {
            other._manager_ptr()->move_to(other, *this);
        }
    }

private:
    alignas(base_manager) char _base_manager_buffer[sizeof(base_manager)];
    char* _storage;
    int _max_size;
    int16_t _max_alignment;
    bool _manager_created;

    [[nodiscard]] const base_manager* _manager_ptr() const
    {
        return reinterpret_cast<const base_manager*>(_base_manager_buffer);
    }

    [[nodiscard]] base_manager* _manager_ptr()
    {
        return reinterpret_cast<base_manager*>(_base_manager_buffer);
    }
};


template<int MaxSize, int MaxAlignment>
class any : public iany
{
    static_assert(MaxSize > 0);
    static_assert(MaxAlignment > 0 && MaxAlignment <= numeric_limits<int16_t>::max());

public:
    any() :
        iany(_storage_buffer, MaxSize, MaxAlignment)
    {
    }

    any(const any& other) :
        any()
    {
        _assign(other);
    }

    any(any&& other) :
        any()
    {
        _assign(move(other));
    }

    any(const iany& other) :
        any()
    {
        _assign(other);
    }

    any(iany&& other) :
        any()
    {
        _assign(move(other));
    }

    template<typename Type>
    explicit any(const Type& value) :
        any()
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        ::new(_value_ptr<Type>()) Type(value);
        _create_manager<Type>();
    }

    template<typename Type>
    explicit any(Type&& value) :
        any()
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        ::new(_value_ptr<Type>()) Type(move(value));
        _create_manager<Type>();
    }

    any& operator=(const any& other)
    {
        if(this != &other)
        {
            reset();
            _assign(other);
        }

        return *this;
    }

    any& operator=(any&& other)
    {
        if(this != &other)
        {
            reset();
            _assign(move(other));
        }

        return *this;
    }

    any& operator=(const iany& other)
    {
        if(this != &other)
        {
            reset();
            _assign(other);
        }

        return *this;
    }

    any& operator=(iany&& other)
    {
        if(this != &other)
        {
            reset();
            _assign(move(other));
        }

        return *this;
    }

    template<typename Type>
    any& operator=(const Type& value)
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        reset();
        ::new(_value_ptr<Type>()) Type(value);
        _create_manager<Type>();
        return *this;
    }

    template<typename Type>
    any& operator=(Type&& value)
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        reset();
        ::new(_value_ptr<Type>()) Type(move(value));
        _create_manager<Type>();
        return *this;
    }

    template<typename Type, typename... Args>
    void emplace(Args&&... args)
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        reset();
        ::new(_value_ptr<Type>()) Type(forward<Args>(args)...);
        _create_manager<Type>();
    }

private:
    alignas(MaxAlignment) char _storage_buffer[MaxSize];
};


template<typename Type>
[[nodiscard]] Type any_cast(const iany& any)
{
    return any.value<Type>();
}

template<typename Type>
[[nodiscard]] Type any_cast(iany& any)
{
    return any.value<Type>();
}

template<typename Type>
[[nodiscard]] Type any_cast(iany&& any)
{
    return move(any.value<Type>());
}

template<typename Type>
[[nodiscard]] const Type* any_cast(const iany* any)
{
    return &(any->value<Type>());
}

template<typename Type>
[[nodiscard]] Type* any_cast(iany* any)
{
    return &(any->value<Type>());
}

}

#endif
