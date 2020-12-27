#include "bn_core.h"
#include "bn_colors.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "../hw/3rd_party/dldi/include/io_scsd.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::colors::gray);

    bn::string_view info_text_lines[] = {
        _SCSD_isInserted() ? "SCSD should be inserted" : "SCSD should not be inserted"
    };

    info information("SCSD test", info_text_lines, text_generator);
    information.set_show_always(true);

    while(true)
    {
        bn::core::update();
    }
}
