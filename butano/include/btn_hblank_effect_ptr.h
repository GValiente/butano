#ifndef BTN_HBLANK_EFFECT_PTR_H
#define BTN_HBLANK_EFFECT_PTR_H

#include "btn_utility.h"
#include "btn_span_fwd.h"
#include "btn_functional.h"

namespace btn
{

class hblank_effect_ptr
{

public:
    hblank_effect_ptr(const hblank_effect_ptr& other);

    hblank_effect_ptr& operator=(const hblank_effect_ptr& other);

    hblank_effect_ptr(hblank_effect_ptr&& other) :
        hblank_effect_ptr(other._id)
    {
        other._id = -1;
    }

    hblank_effect_ptr& operator=(hblank_effect_ptr&& other)
    {
        swap(_id, other._id);
        return *this;
    }

    ~hblank_effect_ptr()
    {
        if(_id >= 0)
        {
            _destroy();
        }
    }

    [[nodiscard]] int id() const
    {
        return _id;
    }

    [[nodiscard]] bool visible() const;

    void set_visible(bool visible);

    [[nodiscard]] friend bool operator==(const hblank_effect_ptr& a, const hblank_effect_ptr& b)
    {
        return a._id == b._id;
    }

    [[nodiscard]] friend bool operator!=(const hblank_effect_ptr& a, const hblank_effect_ptr& b)
    {
        return ! (a == b);
    }

protected:
    explicit hblank_effect_ptr(int id) :
        _id(int8_t(id))
    {
    }

private:
    int8_t _id;

    void _destroy();
};


template<>
struct hash<hblank_effect_ptr>
{
    [[nodiscard]] unsigned operator()(const hblank_effect_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
