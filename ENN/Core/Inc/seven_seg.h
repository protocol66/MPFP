#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

#include <stdint.h>
#include "gpio.h"

struct Seven_Seg_ {
    GPIO a;
    GPIO b;
    GPIO c;
    GPIO d;
    GPIO e;
    GPIO f;
    GPIO g;
    GPIO dot;
    int active_low;        // 0 = active high, 1 = active low
};
typedef struct Seven_Seg_ Seven_Seg;


void fn_seven_seg_init(Seven_Seg* seven_seg)   {
    fn_setup_gpio(&seven_seg->a);
    fn_setup_gpio(&seven_seg->b);
    fn_setup_gpio(&seven_seg->c);
    fn_setup_gpio(&seven_seg->d);
    fn_setup_gpio(&seven_seg->e);
    fn_setup_gpio(&seven_seg->f);
    fn_setup_gpio(&seven_seg->g);
    fn_setup_gpio(&seven_seg->dot);
}

void fn_seven_seg_clear(Seven_Seg* seven_seg) {
    SET_GPIO_PIN_VAL(seven_seg->a, seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->b, seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->c, seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->d, seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->e, seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->f, seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->g, seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->dot, seven_seg->active_low);
}

void fn_seven_seg_set_all(Seven_Seg* seven_seg) {
    SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
    SET_GPIO_PIN_VAL(seven_seg->dot, !seven_seg->active_low);
}

void fn_seven_seg_display(Seven_Seg* seven_seg, uint8_t val, uint8_t dot)    {
    fn_seven_seg_clear(seven_seg);
    SET_GPIO_PIN_VAL(seven_seg->dot, dot ^ seven_seg->active_low);
    switch(val) {
        case 0:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            break;
        case 1:
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            break;
        case 2:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 3:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 4:
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 5:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 6:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 7:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            break;
        case 8:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 9:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 10:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 11:
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 12:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            break;
        case 13:
            SET_GPIO_PIN_VAL(seven_seg->b, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->c, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 14:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->d, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        case 15:
            SET_GPIO_PIN_VAL(seven_seg->a, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->e, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->f, !seven_seg->active_low);
            SET_GPIO_PIN_VAL(seven_seg->g, !seven_seg->active_low);
            break;
        default:
            break;
    }
}

#endif