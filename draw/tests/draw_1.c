#include "../draw.h"
#include "../../img/img.h"
#include "../../colors/colors.h"

char const *_name_s = "draw_1.rwh";
int32_t const img_w = 300,
        img_h = 300;

int main (void) {
    draw *_d = malloc(sizeof(draw));
    if (! draw_cons (_d, img_w, img_h))
        return 1;
    
    img *_i = malloc(sizeof(img));
    if (! img_cons (_i))
        return 2;
    _i->w = _d->w;
    _i->h = _d->h;
    _i->_b = (uint8_t *)_d->_p;
    _i->b_n = _d->w * _d->h * 4;

    draw_clear (_d, COLOR_blue);
    draw_rect (_d, (_i->w / 2) - 20, (_i->h / 2) - 20, 40, 40, COLOR_green);
    draw_line (_d, 0, 0, _d->w - 1, _d->h - 1, COLOR_red);

    
    if (! img_swap_r_b (_i))
        return 3;

    if (! img_save_rwh (_i, _name_s))
        return 4;
    
    if (! img_decons (_i))
        return 5;

    if (! draw_decons (_d))
        return 6;

    free(_i);
    return 0;
}

