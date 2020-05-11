#ifndef BTN_VARIANT_H
#define BTN_VARIANT_H

#include <new>
#include "btn_assert.h"
#include "btn_utility.h"
#include "btn_algorithm.h"
#include "btn_functional.h"

namespace btn
{

inline constexpr int variant_npos = -1;

template<typename... Types>
class variant
{

private:
    template<int Index, typename Type, typename OtherType, typename... OtherTypes>
    struct variant_index
    {
        static constexpr int index =
                is_same_v<Type, OtherType> ? Index : variant_index<Index + 1, Type, OtherTypes...>::index;
    };

    template<int Index, typename Type, typename OtherType>
    struct variant_index<Index, Type, OtherType>
    {
        static constexpr int index = is_same_v<Type, OtherType> ? Index : 0;
    };

    template<typename Type, typename... OtherTypes>
    struct variant_copy
    {
        variant_copy(int index, int assigned, const void* source, void* target)
        {
            if(index == assigned)
            {
                auto typed_source = static_cast<const Type*>(source);
                new(target) Type(*typed_source);
            }
            else
            {
                variant_copy<OtherTypes...>(index + 1, assigned, source, target);
            }
        }
    };

    template<typename Type>
    struct variant_copy<Type>
    {
        variant_copy(int index, int assigned, const void* source, void* target)
        {
            if(index == assigned)
            {
                auto typed_source = static_cast<const Type*>(source);
                new(target) Type(*typed_source);
            }
        }
    };

    template<typename Type, typename... OtherTypes>
    struct variant_move
    {
        variant_move(int index, int assigned, void* source, void* target)
        {
            if(index == assigned)
            {
                auto typed_source = static_cast<Type*>(source);
                new(target) Type(move(*typed_source));
                typed_source->~Type();
            }
            else
            {
                variant_move<OtherTypes...>(index + 1, assigned, source, target);
            }
        }
    };

    template<typename Type>
    struct variant_move<Type>
    {
        variant_move(int index, int assigned, void* source, void* target)
        {
            if(index == assigned)
            {
                auto typed_source = static_cast<Type*>(source);
                new(target) Type(move(*typed_source));
                typed_source->~Type();
            }
        }
    };

    template<typename Type, typename... OtherTypes>
    struct variant_swap
    {
        variant_swap(int index, int assigned, void* source, void* target)
        {
            if(index == assigned)
            {
                auto typed_source = static_cast<Type*>(source);
                auto typed_target = static_cast<Type*>(target);
                btn::swap(*typed_source, *typed_target);
            }
            else
            {
                variant_swap<OtherTypes...>(index + 1, assigned, source, target);
            }
        }
    };

    template<typename Type>
    struct variant_swap<Type>
    {
        variant_swap(int index, int assigned, void* source, void* target)
        {
            if(index == assigned)
            {
                auto typed_source = static_cast<Type*>(source);
                auto typed_target = static_cast<Type*>(target);
                btn::swap(*typed_source, *typed_target);
            }
        }
    };

    template<typename Type, typename... OtherTypes>
    struct variant_destruct
    {
        variant_destruct(int index, int assigned, void* data)
        {
            if(index == assigned)
            {
                auto typed_data = static_cast<Type*>(data);
                typed_data->~Type();
            }
            else
            {
                variant_destruct<OtherTypes...>(index + 1, assigned, data);
            }
        }
    };

    template<typename Type>
    struct variant_destruct<Type>
    {
        variant_destruct(int index, int assigned, void* data)
        {
            if(index == assigned)
            {
                auto typed_data = static_cast<Type*>(data);
                typed_data->~Type();
            }
        }
    };

    template<typename Type, typename OtherType, typename... OtherTypes>
    struct variant_same_type
    {
        [[nodiscard]] constexpr bool operator()(int index, int assigned) const
        {
            if(index == assigned)
            {
                return is_same_v<Type, OtherType>;
            }

            return variant_same_type<Type, OtherTypes...>()(index + 1, assigned);
        }
    };

    template<typename Type, typename OtherType>
    struct variant_same_type<Type, OtherType>
    {
        [[nodiscard]] constexpr bool operator()(int index, int assigned) const
        {
            if(index == assigned)
            {
                return is_same_v<Type, OtherType>;
            }

            return false;
        }
    };

    template<typename Type, typename... OtherTypes>
    struct variant_hash
    {
        [[nodiscard]] unsigned operator()(int index, int assigned, const void* data) const
        {
            if(index == assigned)
            {
                auto typed_data = static_cast<const Type*>(data);
                unsigned result = make_hash(index);
                hash_combine(*typed_data, result);
                return result;
            }

            return variant_hash<OtherTypes...>()(index + 1, assigned, data);
        }
    };

    template<typename Type>
    struct variant_hash<Type>
    {
        [[nodiscard]] unsigned operator()(int index, int assigned, const void* data) const
        {
            if(index == assigned)
            {
                auto typed_data = static_cast<const Type*>(data);
                unsigned result = make_hash(index);
                hash_combine(*typed_data, result);
                return result;
            }

            return make_hash(0);
        }
    };

    template<typename Type, typename... OtherTypes>
    struct variant_equals
    {
        [[nodiscard]] bool operator()(int index, int assigned, const void* a, const void* b) const
        {
            if(index == assigned)
            {
                auto typed_a = static_cast<const Type*>(a);
                auto typed_b = static_cast<const Type*>(b);
                return *typed_a == *typed_b;
            }

            return variant_equals<OtherTypes...>()(index + 1, assigned, a, b);
        }
    };

    template<typename Type>
    struct variant_equals<Type>
    {
        [[nodiscard]] bool operator()(int index, int assigned, const void* a, const void* b) const
        {
            if(index == assigned)
            {
                auto typed_a = static_cast<const Type*>(a);
                auto typed_b = static_cast<const Type*>(b);
                return *typed_a == *typed_b;
            }

            return false;
        }
    };

    template<typename Type, typename... OtherTypes>
    struct variant_less
    {
        [[nodiscard]] bool operator()(int index, int assigned, const void* a, const void* b) const
        {
            if(index == assigned)
            {
                auto typed_a = static_cast<const Type*>(a);
                auto typed_b = static_cast<const Type*>(b);
                return *typed_a < *typed_b;
            }

            return variant_less<OtherTypes...>()(index + 1, assigned, a, b);
        }
    };

    template<typename Type>
    struct variant_less<Type>
    {
        [[nodiscard]] bool operator()(int index, int assigned, const void* a, const void* b) const
        {
            if(index == assigned)
            {
                auto typed_a = static_cast<const Type*>(a);
                auto typed_b = static_cast<const Type*>(b);
                return *typed_a < *typed_b;
            }

            return false;
        }
    };

public:
    variant() = default;

    variant(const variant& other)
    {
        if(other._assigned)
        {
            variant_copy<Types...>(1, other._assigned, other._storage, _storage);
            _assigned = other._assigned;
        }
        else
        {
            _assigned = 0;
        }
    }

    variant(variant&& other)
    {
        if(other._assigned)
        {
            variant_move<Types...>(1, other._assigned, other._storage, _storage);
            _assigned = other._assigned;
            other._assigned = 0;
        }
        else
        {
            _assigned = 0;
        }
    }

    template<typename Type>
    variant(const Type& value)
    {
        static_assert(variant_index<1, Type, Types...>::index);

        new(_storage) Type(value);
        _assigned = variant_index<1, Type, Types...>::index;
    }

    template<typename Type>
    variant(Type&& value)
    {
        static_assert(variant_index<1, Type, Types...>::index);

        _clean();

        new(_storage) Type(move(value));
        _assigned = variant_index<1, Type, Types...>::index;
    }

    variant& operator=(const variant& other)
    {
        if(this != &other)
        {
            if(other._assigned)
            {
                _clean();
                variant_copy<Types...>(1, other._assigned, other._storage, _storage);
                _assigned = other._assigned;
            }
            else
            {
                reset();
            }
        }

        return *this;
    }

    variant& operator=(variant&& other)
    {
        if(this != &other)
        {
            if(other._assigned)
            {
                _clean();
                variant_move<Types...>(1, other._assigned, other._storage, _storage);
                _assigned = other._assigned;
                other._assigned = 0;
            }
            else
            {
                reset();
            }
        }

        return *this;
    }

    ~variant()
    {
        _clean();
    }

    [[nodiscard]] bool valueless_by_exception() const
    {
        return _assigned == 0;
    }

    template<typename Type>
    [[nodiscard]] bool holds_alternative() const
    {
        return _assigned && variant_same_type<Type, Types...>()(1, _assigned);
    }

    template<typename Type>
    [[nodiscard]] int index() const
    {
        if(holds_alternative<Type>())
        {
            return variant_index<1, Type, Types...>::index - 1;
        }

        return variant_npos;
    }

    template<typename Type>
    [[nodiscard]] const Type& get() const
    {
        static_assert(variant_index<1, Type, Types...>::index);

        BTN_ASSERT(holds_alternative<Type>(), "Variant does not contains the specified type");

        auto typed_storage = reinterpret_cast<const Type*>(_storage);
        return *typed_storage;
    }

    template<typename Type>
    [[nodiscard]] Type& get()
    {
        static_assert(variant_index<1, Type, Types...>::index);

        BTN_ASSERT(holds_alternative<Type>(), "Variant does not contains the specified type");

        auto typed_storage = reinterpret_cast<Type*>(_storage);
        return *typed_storage;
    }

    template<typename Type>
    [[nodiscard]] const Type* get_if() const
    {
        static_assert(variant_index<1, Type, Types...>::index);

        if(holds_alternative<Type>())
        {
            auto typed_storage = reinterpret_cast<const Type*>(_storage);
            return *typed_storage;
        }

        return nullptr;
    }

    template<typename Type>
    [[nodiscard]] Type* get_if()
    {
        static_assert(variant_index<1, Type, Types...>::index);

        if(holds_alternative<Type>())
        {
            auto typed_storage = reinterpret_cast<Type*>(_storage);
            return *typed_storage;
        }

        return nullptr;
    }

    template<typename Type>
    variant& operator=(const Type& value)
    {
        static_assert(variant_index<1, Type, Types...>::index);

        _clean();

        new(_storage) Type(value);
        _assigned = variant_index<1, Type, Types...>::index;
        return *this;
    }

    template<typename Type>
    variant& operator=(Type&& value)
    {
        static_assert(variant_index<1, Type, Types...>::index);

        _clean();

        new(_storage) Type(move(value));
        _assigned = variant_index<1, Type, Types...>::index;
        return *this;
    }

    template<typename Type, typename... Args>
    Type& emplace(Args&&... args)
    {
        static_assert(variant_index<1, Type, Types...>::index);

        _clean();

        auto typed_storage = reinterpret_cast<Type*>(_storage);
        new(typed_storage) Type(forward<Args>(args)...);
        _assigned = variant_index<1, Type, Types...>::index;
        return *typed_storage;
    }

    void reset()
    {
        if(_assigned)
        {
            variant_destruct<Types...>(1, _assigned, _storage);
            _assigned = 0;
        }
    }

    [[nodiscard]] unsigned hash() const
    {
        if(_assigned)
        {
            return variant_hash<Types...>()(1, _assigned, _storage);
        }

        return make_hash(0);
    }

    void swap(variant& other)
    {
        if(_assigned)
        {
            if(other._assigned)
            {
                if(_assigned == other._assigned)
                {
                    variant_swap<Types...>(1, _assigned, _storage, other._storage);
                }
                else
                {
                    variant temp = move(*this);
                    *this = move(other);
                    other = move(temp);
                }
            }
            else
            {
                other = move(*this);
            }
        }
        else
        {
            if(other._assigned)
            {
                *this = move(other);
            }
        }
    }

    friend void swap(variant& a, variant& b)
    {
        a.swap(b);
    }

    [[nodiscard]] friend bool operator==(const variant& a, const variant& b)
    {
        if(a._assigned != b._assigned)
        {
            return false;
        }

        if(! a._assigned)
        {
            return true;
        }

        return variant_equals<Types...>()(1, a._assigned, a._storage, b._storage);
    }

    [[nodiscard]] friend bool operator!=(const variant& a, const variant& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] friend bool operator<(const variant& a, const variant& b)
    {
        if(! b._assigned)
        {
            return false;
        }

        if(! a._assigned)
        {
            return true;
        }

        if(a._assigned < b._assigned)
        {
            return true;
        }

        if(a._assigned > b._assigned)
        {
            return false;
        }

        return variant_less<Types...>()(1, a._assigned, a._storage, b._storage);
    }

    [[nodiscard]] friend bool operator>(const variant& a, const variant& b)
    {
        return b < a;
    }

    [[nodiscard]] friend bool operator<=(const variant& a, const variant& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] friend bool operator>=(const variant& a, const variant& b)
    {
        return ! (a < b);
    }

private:
    template<typename Type, typename... OtherTypes>
    struct variant_size
    {
        static constexpr int size = max(int(sizeof(Type)), variant_size<OtherTypes...>::size);
    };

    template<typename Type>
    struct variant_size<Type>
    {
        static constexpr int size = sizeof(Type);
    };

    alignas(Types...) char _storage[variant_size<Types...>::size];
    int8_t _assigned = 0;

    void _clean()
    {
        if(_assigned)
        {
            variant_destruct<Types...>(1, _assigned, _storage);
        }
    }
};


template<typename... Types>
struct hash<variant<Types...>>
{
    [[nodiscard]] unsigned operator()(const variant<Types...>& variant) const
    {
        return variant.hash();
    }
};

}

#endif
