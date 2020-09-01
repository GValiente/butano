#ifndef BTN_WINDOW_H
#define BTN_WINDOW_H

#include "btn_functional.h"

namespace btn
{

class regular_bg_ptr;

class window
{

public:
    [[nodiscard]] static window internal()
    {
        return window(0);
    }

    [[nodiscard]] static window external()
    {
        return window(1);
    }

    [[nodiscard]] static window sprites()
    {
        return window(2);
    }

    [[nodiscard]] static window outside()
    {
        return window(3);
    }

    [[nodiscard]] int id() const
    {
        return _id;
    }

    [[nodiscard]] bool show_bg(const regular_bg_ptr& regular_bg) const;

    void set_show_bg(const regular_bg_ptr& regular_bg, bool show);

    [[nodiscard]] bool show_sprites() const;

    void set_show_sprites(bool show);

    [[nodiscard]] bool show_blending() const;

    void set_show_blending(bool show);

    [[nodiscard]] friend bool operator==(const window& a, const window& b) = default;

protected:
    explicit window(int id) :
        _id(int8_t(id))
    {
    }

private:
    int8_t _id;
};


template<>
struct hash<window>
{
    [[nodiscard]] unsigned operator()(const window& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
