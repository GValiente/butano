/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ANY_H
#define BN_ANY_H

/**
 * @file
 * bn::iany and bn::any implementation header file.
 *
 * @ingroup any
 */

#include <new>
#include "bn_assert.h"
#include "bn_limits.h"
#include "bn_type_id.h"
#include "bn_utility.h"
#include "bn_any_fwd.h"

namespace bn
{

/**
 * @brief Base class of bn::any.
 *
 * Can be used as a reference type for all bn::any containers.
 *
 * @ingroup any
 */
class iany
{

public:
    iany(const iany& other) = delete;

    /**
     * @brief Destructor.
     */
    ~iany() noexcept
    {
        reset();
    }

    /**
     * @brief Copy assignment operator.
     * @param other iany to copy.
     * @return Reference to this.
     */
    iany& operator=(const iany& other)
    {
        if(this != &other)
        {
            reset();
            _assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other iany to move.
     * @return Reference to this.
     */
    iany& operator=(iany&& other) noexcept
    {
        if(this != &other)
        {
            reset();
            _assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Constructor.
     * @param value Constant value to copy.
     */
    template<typename Type>
    iany& operator=(const Type& value)
    {
        BN_ASSERT(int(sizeof(Type)) <= _max_size, "Invalid value size: ", sizeof(Type), " - ", _max_size);
        BN_ASSERT(int(alignof(Type)) <= _max_alignment, "Invalid value alignment: ",
                  alignof(Type), " - ", _max_alignment);

        reset();
        new(_value_ptr<Type>()) Type(value);
        _create_manager<Type>();
        return *this;
    }

    /**
     * @brief Constructor.
     * @param value Non constant value to copy.
     */
    template<typename Type>
    iany& operator=(Type& value)
    {
        BN_ASSERT(int(sizeof(Type)) <= _max_size, "Invalid value size: ", sizeof(Type), " - ", _max_size);
        BN_ASSERT(int(alignof(Type)) <= _max_alignment, "Invalid value alignment: ",
                  alignof(Type), " - ", _max_alignment);

        reset();
        new(_value_ptr<Type>()) Type(value);
        _create_manager<Type>();
        return *this;
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     */
    template<typename Type>
    iany& operator=(Type&& value) noexcept
    {
        BN_ASSERT(int(sizeof(Type)) <= _max_size, "Invalid value size: ", sizeof(Type), " - ", _max_size);
        BN_ASSERT(int(alignof(Type)) <= _max_alignment, "Invalid value alignment: ",
                   alignof(Type), " - ", _max_alignment);

        reset();
        new(_value_ptr<Type>()) Type(bn::forward<Type>(value));
        _create_manager<Type>();
        return *this;
    }

    /**
     * @brief Indicates if it contains a value or not.
     */
    [[nodiscard]] bool has_value() const
    {
        return _manager_created;
    }

    /**
     * @brief Indicates if it contains a value or not.
     */
    [[nodiscard]] explicit operator bool() const
    {
        return _manager_created;
    }

    /**
     * @brief Returns the type_id_t of the contained value if any, otherwise type_id_t().
     */
    [[nodiscard]] type_id_t type() const
    {
        return has_value() ? _manager_ptr()->type() : type_id_t();
    }

    /**
     * @brief Returns the maximum size in bytes of the managed objects.
     */
    [[nodiscard]] int max_size() const
    {
        return _max_size;
    }

    /**
     * @brief Returns the maximum alignment in bytes of the managed objects.
     */
    [[nodiscard]] int max_alignment() const
    {
        return _max_alignment;
    }

    /**
     * @brief Returns a const reference to the contained value.
     * @tparam Type Type of the contained value.
     */
    template<typename Type>
    [[nodiscard]] const Type& value() const
    {
        BN_ASSERT(type() == type_id<Type>(), "Invalid value type");

        return *_value_ptr<Type>();
    }
    /**
     * @brief Returns a reference to the contained value.
     * @tparam Type Type of the contained value.
     */
    template<typename Type>
    [[nodiscard]] Type& value()
    {
        BN_ASSERT(type() == type_id<Type>(), "Invalid value type");

        return *_value_ptr<Type>();
    }

    /**
     * @brief Returns a copy of the contained value if this iany has one;
     * otherwise it returns a copy of the given default value.
     * @tparam Type Type of the contained value.
     */
    template<typename Type>
    [[nodiscard]] Type value_or(const Type& default_value) const
    {
        return has_value() ? value<Type>() : default_value;
    }

    /**
     * @brief Returns a copy of the contained value if this iany has one;
     * otherwise it returns a moved copy of the given default value.
     * @tparam Type Type of the contained value.
     */
    template<typename Type>
    [[nodiscard]] Type value_or(Type&& default_value) const
    {
        return has_value() ? value<Type>() : bn::forward<Type>(default_value);
    }

    /**
     * @brief Constructs the contained value in-place.
     * @tparam Type Type of the value to construct.
     * @tparam Args Types of the parameters of the value to construct.
     * @param args Parameters of the value to construct.
     * @return Reference to the constructed value.
     */
    template<typename Type, typename... Args>
    Type& emplace(Args&&... args)
    {
        BN_ASSERT(int(sizeof(Type)) <= _max_size, "Invalid value size: ", sizeof(Type), " - ", _max_size);
        BN_ASSERT(int(alignof(Type)) <= _max_alignment, "Invalid value alignment: ",
                  alignof(Type), " - ", _max_alignment);

        reset();

        Type* result = _value_ptr<Type>();
        new(result) Type(forward<Args>(args)...);
        _create_manager<Type>();
        return *result;
    }

    /**
     * @brief Disposes the contained value.
     */
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

    /**
     * @brief Exchanges the contents of this iany with those of the other one.
     * @param other iany to exchange the contents with.
     */
    void swap(iany& other)
    {
        if(has_value())
        {
            if(other.has_value())
            {
                base_manager* manager = _manager_ptr();
                BN_ASSERT(manager->type() == other._manager_ptr()->type(), "Value type mismatch");

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

    /**
     * @brief Exchanges the contents of a iany with those of another one.
     * @param a First iany to exchange the contents with.
     * @param b Second iany to exchange the contents with.
     */
    friend void swap(iany& a, iany& b)
    {
        a.swap(b);
    }

protected:
    /// @cond DO_NOT_DOCUMENT

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
        [[nodiscard]] type_id_t type() const final
        {
            return type_id<Type>();
        }

        void copy_to(const iany& this_any, iany& other_any) const final
        {
            BN_ASSERT(int(sizeof(Type)) <= other_any.max_size(), "Invalid value size: ",
                      sizeof(Type), " - ", other_any.max_size());
            BN_ASSERT(int(alignof(Type)) <= other_any.max_alignment(), "Invalid value alignment: ",
                      alignof(Type), " - ", other_any.max_alignment());

            if constexpr(is_copy_constructible_v<Type>)
            {
                other_any.reset();
                new(other_any._value_ptr<Type>()) Type(*this_any._value_ptr<Type>());
                other_any._create_manager<Type>();
            }
            else
            {
                BN_ERROR("This type can't be copied");
            }
        }

        void move_to(iany& this_any, iany& other_any) const final
        {
            BN_ASSERT(int(sizeof(Type)) <= other_any.max_size(), "Invalid value size: ",
                      sizeof(Type), " - ", other_any.max_size());
            BN_ASSERT(int(alignof(Type)) <= other_any.max_alignment(), "Invalid value alignment: ",
                      alignof(Type), " - ", other_any.max_alignment());

            if constexpr(is_move_constructible_v<Type>)
            {
                other_any.reset();
                new(other_any._value_ptr<Type>()) Type(move(*this_any._value_ptr<Type>()));
                other_any._create_manager<Type>();
                this_any.reset();
            }
            else
            {
                BN_ERROR("This type can't be moved");
            }
        }

        void swap(iany& this_any, iany& other_any) const final
        {
            if constexpr(is_swappable_v<Type>)
            {
                bn::swap(*this_any._value_ptr<Type>(), *other_any._value_ptr<Type>());
            }
            else
            {
                BN_ERROR("This type can't be swapped");
            }
        }

        void destroy(iany& any) const final
        {
            any._value_ptr<Type>()->~Type();
        }
    };

    iany(char* storage, int max_size, int max_alignment) :
        _storage(storage),
        _max_size(max_size),
        _max_alignment(int16_t(max_alignment))
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

        new(_manager_ptr()) type_manager<Type>();
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

    /// @endcond

private:
    alignas(base_manager) char _base_manager_buffer[sizeof(base_manager)];
    char* _storage;
    int _max_size;
    int16_t _max_alignment;
    bool _manager_created = false;

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
    /**
     * @brief Default constructor.
     */
    any() :
        iany(_storage_buffer, MaxSize, MaxAlignment)
    {
    }

    /**
     * @brief Copy constructor.
     * @param other any to copy.
     */
    any(const any& other) :
        any()
    {
        _assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other any to move.
     */
    any(any&& other) noexcept :
        any()
    {
        _assign(move(other));
    }

    /**
     * @brief Copy constructor.
     * @param other iany to copy.
     */
    any(const iany& other) :
        any()
    {
        _assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other iany to move.
     */
    any(iany&& other) noexcept :
        any()
    {
        _assign(move(other));
    }

    /**
     * @brief Constructor.
     * @param value Constant value to copy.
     */
    template<typename Type>
    explicit any(const Type& value) :
        any()
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        new(_value_ptr<Type>()) Type(value);
        _create_manager<Type>();
    }

    /**
     * @brief Constructor.
     * @param value Non constant value to copy.
     */
    template<typename Type>
    explicit any(Type& value) :
        any()
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        new(_value_ptr<Type>()) Type(value);
        _create_manager<Type>();
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     */
    template<typename Type>
    explicit any(Type&& value) :
        any()
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        new(_value_ptr<Type>()) Type(bn::forward<Type>(value));
        _create_manager<Type>();
    }

    /**
     * @brief Copy assignment operator.
     * @param other any to copy.
     * @return Reference to this.
     */
    any& operator=(const any& other)
    {
        if(this != &other)
        {
            reset();
            _assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other any to move.
     * @return Reference to this.
     */
    any& operator=(any&& other) noexcept
    {
        if(this != &other)
        {
            reset();
            _assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other iany to copy.
     * @return Reference to this.
     */
    any& operator=(const iany& other)
    {
        if(this != &other)
        {
            reset();
            _assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other iany to move.
     * @return Reference to this.
     */
    any& operator=(iany&& other) noexcept
    {
        if(this != &other)
        {
            reset();
            _assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Assignment operator.
     * @param value Constant value to copy.
     * @return Reference to this.
     */
    template<typename Type>
    any& operator=(const Type& value)
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        reset();
        new(_value_ptr<Type>()) Type(value);
        _create_manager<Type>();
        return *this;
    }

    /**
     * @brief Assignment operator.
     * @param value Non constant value to copy.
     * @return Reference to this.
     */
    template<typename Type>
    any& operator=(Type& value)
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        reset();
        new(_value_ptr<Type>()) Type(value);
        _create_manager<Type>();
        return *this;
    }

    /**
     * @brief Assignment operator.
     * @param value Value to move.
     * @return Reference to this.
     */
    template<typename Type>
    any& operator=(Type&& value)
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        reset();
        new(_value_ptr<Type>()) Type(bn::forward<Type>(value));
        _create_manager<Type>();
        return *this;
    }

    /**
     * @brief Constructs the contained value in-place.
     * @tparam Type Type of the value to construct.
     * @tparam Args Types of the parameters of the value to construct.
     * @param args Parameters of the value to construct.
     * @return Reference to the constructed value.
     */
    template<typename Type, typename... Args>
    Type& emplace(Args&&... args)
    {
        static_assert(int(sizeof(Type)) <= MaxSize);
        static_assert(int(alignof(Type)) <= MaxAlignment);

        reset();

        Type* result = _value_ptr<Type>();
        new(result) Type(forward<Args>(args)...);
        _create_manager<Type>();
        return *result;
    }

private:
    static constexpr unsigned _alignment = MaxAlignment > alignof(int) ? MaxAlignment : alignof(int);

    alignas(_alignment) char _storage_buffer[MaxSize];
};


/**
 * @brief Performs type-safe access to the contained object of the given iany.
 * @tparam Type Type of the contained value.
 * @param any Target iany object.
 * @return If the given iany pointer is not null,
 * and the type_id_t of the requested Type matches the type_id_t of the given iany,
 * a const pointer to the value contained by operand; otherwise `nullptr`.
 *
 * @ingroup any
 */
template<typename Type>
[[nodiscard]] const Type* any_cast(const iany* any)
{
    if(any && any->type() == type_id<Type>())
    {
        return &(any->value<Type>());
    }

    return nullptr;
}

/**
 * @brief Performs type-safe access to the contained object of the given iany.
 * @tparam Type Type of the contained value.
 * @param any Target iany object.
 * @return If the given iany pointer is not null,
 * and the type_id_t of the requested Type matches the type_id_t of the given iany,
 * a pointer to the value contained by operand; otherwise `nullptr`.
 *
 * @ingroup any
 */
template<typename Type>
[[nodiscard]] Type* any_cast(iany* any)
{
    if(any && any->type() == type_id<Type>())
    {
        return &(any->value<Type>());
    }

    return nullptr;
}

/**
 * @brief Creates an any deducing its size and alignment from the given Type.
 * @tparam Type Type of the value to construct.
 * @tparam Args Types of the parameters of the value to construct.
 * @param args Parameters of the value to construct.
 * @return any containing to the constructed value.
 *
 * @ingroup any
 */
template<typename Type, typename... Args>
any<sizeof(Type), alignof(Type)> make_any(Args&&... args)
{
    any<sizeof(Type), alignof(Type)> result;
    result.emplace(forward<Args>(args)...);
    return result;
}

}

#endif
