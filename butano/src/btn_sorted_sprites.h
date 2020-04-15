#ifndef BTN_SORTED_SPRITES_H
#define BTN_SORTED_SPRITES_H

#include "btn_vector_fwd.h"
#include "btn_intrusive_list.h"
#include "btn_sprites_manager_item.h"

namespace btn::sorted_sprites
{
    class layer : public intrusive_list<sprites_manager_item>
    {

    public:
        explicit layer(unsigned sort_key) :
            intrusive_list<sprites_manager_item>(),
            _sort_key(sort_key)
        {
        }

        [[nodiscard]] unsigned sort_key() const
        {
            return _sort_key;
        }

    private:
        unsigned _sort_key;
    };


    using layers_type = ivector<layer*>;


    [[nodiscard]] int items_count();

    [[nodiscard]] layers_type& layers();

    void insert(sprites_manager_item& item);

    void erase(sprites_manager_item& item);
}

#endif
