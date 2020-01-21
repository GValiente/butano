#ifndef BTN_BG_MAP_PTR_H
#define BTN_BG_MAP_PTR_H

#include "btn_span_fwd.h"
#include "btn_functional.h"
#include "btn_bg_map_cell.h"
#include "btn_optional_fwd.h"

namespace btn
{

class size;

class bg_map_ptr
{

public:
    [[nodiscard]] static optional<bg_map_ptr> find(const bg_map_cell& cells_ref, const size& dimensions);

    [[nodiscard]] static bg_map_ptr create(const bg_map_cell& cells_ref, const size& dimensions);

    [[nodiscard]] static bg_map_ptr find_or_create(const bg_map_cell& cells_ref, const size& dimensions);

    [[nodiscard]] static bg_map_ptr allocate(const size& dimensions);

    [[nodiscard]] static optional<bg_map_ptr> optional_create(const bg_map_cell& cells_ref, const size& dimensions);

    [[nodiscard]] static optional<bg_map_ptr> optional_find_or_create(const bg_map_cell& cells_ref,
                                                                      const size& dimensions);

    [[nodiscard]] static optional<bg_map_ptr> optional_allocate(const size& dimensions);

    bg_map_ptr(const bg_map_ptr& other);

    bg_map_ptr& operator=(const bg_map_ptr& other);

    bg_map_ptr(bg_map_ptr&& other);

    bg_map_ptr& operator=(bg_map_ptr&& other);

    ~bg_map_ptr()
    {
        _destroy();
    }

    [[nodiscard]] int id() const;

    [[nodiscard]] size dimensions() const;

    [[nodiscard]] const bg_map_cell* cells_ref() const;

    void set_cells_ref(const bg_map_cell& cells_ref, const size& dimensions);

    void reload_cells_ref();

    [[nodiscard]] optional<span<bg_map_cell>> vram();

    [[nodiscard]] size_t hash() const
    {
        return make_hash(_handle);
    }

    [[nodiscard]] friend bool operator==(const bg_map_ptr& a, const bg_map_ptr& b)
    {
        return a._handle == b._handle;
    }

    [[nodiscard]] friend bool operator!=(const bg_map_ptr& a, const bg_map_ptr& b)
    {
        return ! (a == b);
    }

private:
    int8_t _handle;

    explicit bg_map_ptr(int handle) :
        _handle(int8_t(handle))
    {
    }

    void _destroy();
};


template<>
struct hash<bg_map_ptr>
{
    [[nodiscard]] size_t operator()(const bg_map_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
