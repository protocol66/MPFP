#ifndef KEYPAD_H
#define KEYPAD_H

#include "common.h"
#include "gpio.h"


#define GPIO_DELAY 10   // delay for gpio to settle in microseconds

const GPIO ROW0 = { .pst_port = GPIOC, 
                    .u32_pin = LL_GPIO_PIN_7, 
                    .u32_mode = LL_GPIO_MODE_INPUT, 
                    .u32_up_down = LL_GPIO_PULL_UP, 
                    .u32_type = LL_GPIO_OUTPUT_OPENDRAIN, 
                    .u32_speed = LL_GPIO_SPEED_FREQ_LOW, 
                    .u32_af = LL_GPIO_AF_0};

const GPIO ROW1 = { .pst_port = GPIOA,
                    .u32_pin = LL_GPIO_PIN_9,
                    .u32_mode = LL_GPIO_MODE_INPUT,
                    .u32_up_down = LL_GPIO_PULL_UP,
                    .u32_type = LL_GPIO_OUTPUT_OPENDRAIN,
                    .u32_speed = LL_GPIO_SPEED_FREQ_LOW,
                    .u32_af = LL_GPIO_AF_0};

const GPIO ROW2 = { .pst_port = GPIOA,
                    .u32_pin = LL_GPIO_PIN_8,
                    .u32_mode = LL_GPIO_MODE_INPUT,
                    .u32_up_down = LL_GPIO_PULL_UP,
                    .u32_type = LL_GPIO_OUTPUT_OPENDRAIN,
                    .u32_speed = LL_GPIO_SPEED_FREQ_LOW,
                    .u32_af = LL_GPIO_AF_0};

const GPIO ROW3 = { .pst_port = GPIOB,
                    .u32_pin = LL_GPIO_PIN_10,
                    .u32_mode = LL_GPIO_MODE_INPUT,
                    .u32_up_down = LL_GPIO_PULL_UP,
                    .u32_type = LL_GPIO_OUTPUT_OPENDRAIN,
                    .u32_speed = LL_GPIO_SPEED_FREQ_LOW,
                    .u32_af = LL_GPIO_AF_0};

const GPIO COL0 = { .pst_port = GPIOB,
                    .u32_pin = LL_GPIO_PIN_4,
                    .u32_mode = LL_GPIO_MODE_OUTPUT,
                    .u32_up_down = LL_GPIO_PULL_NO,
                    .u32_type = LL_GPIO_OUTPUT_OPENDRAIN,
                    .u32_speed = LL_GPIO_SPEED_FREQ_LOW,
                    .u32_af = LL_GPIO_AF_0};

const GPIO COL1 = { .pst_port = GPIOB,
                    .u32_pin = LL_GPIO_PIN_5,
                    .u32_mode = LL_GPIO_MODE_OUTPUT,
                    .u32_up_down = LL_GPIO_PULL_NO,
                    .u32_type = LL_GPIO_OUTPUT_OPENDRAIN,
                    .u32_speed = LL_GPIO_SPEED_FREQ_LOW,
                    .u32_af = LL_GPIO_AF_0};

const GPIO COL2 = { .pst_port = GPIOB,
                    .u32_pin = LL_GPIO_PIN_3,
                    .u32_mode = LL_GPIO_MODE_OUTPUT,
                    .u32_up_down = LL_GPIO_PULL_NO,
                    .u32_type = LL_GPIO_OUTPUT_OPENDRAIN,
                    .u32_speed = LL_GPIO_SPEED_FREQ_LOW,
                    .u32_af = LL_GPIO_AF_0};

const GPIO COL3 = { .pst_port = GPIOA,
                    .u32_pin = LL_GPIO_PIN_10,
                    .u32_mode = LL_GPIO_MODE_OUTPUT,
                    .u32_up_down = LL_GPIO_PULL_NO,
                    .u32_type = LL_GPIO_OUTPUT_OPENDRAIN,
                    .u32_speed = LL_GPIO_SPEED_FREQ_LOW,
                    .u32_af = LL_GPIO_AF_0};


const char KEY_MAP[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};


__STATIC_INLINE
void RESET_KEYPAD(void) {
    CLEAR_GPIO_PIN(COL0);       // set all columns to low, so that all rows can be read
    CLEAR_GPIO_PIN(COL1);
    CLEAR_GPIO_PIN(COL2);
    CLEAR_GPIO_PIN(COL3);
}


void SETUP_KEYPAD(void)     {
    fn_setup_gpio(&ROW0);
    fn_setup_gpio(&ROW1);
    fn_setup_gpio(&ROW2);
    fn_setup_gpio(&ROW3);
    fn_setup_gpio(&COL0);
    fn_setup_gpio(&COL1);
    fn_setup_gpio(&COL2);
    fn_setup_gpio(&COL3);

    RESET_KEYPAD();
}

__STATIC_INLINE
int IS_ANY_KEY_PRESSED(void)   {
    if (!READ_GPIO_PIN(ROW0) || !READ_GPIO_PIN(ROW1) || !READ_GPIO_PIN(ROW2) || !READ_GPIO_PIN(ROW3)) {
        return 1;
    } else {
        return 0;
    }
}

__STATIC_INLINE
void __SELECT_COLUMN(GPIO st_col)   {
    SET_GPIO_PIN(COL0);         // open drain, so set all columns are floating except the one we want to select
    SET_GPIO_PIN(COL1);
    SET_GPIO_PIN(COL2);
    SET_GPIO_PIN(COL3);
    CLEAR_GPIO_PIN(st_col);
}

__STATIC_INLINE
int __GET_KEY_ROW(void)   {
    if (!READ_GPIO_PIN(ROW0)) {         // button pressed = active low
        return 0;
    } else if (!READ_GPIO_PIN(ROW1)) {
        return 1;
    } else if (!READ_GPIO_PIN(ROW2)) {
        return 2;
    } else if (!READ_GPIO_PIN(ROW3)) {
        return 3;
    } else {
        return -1;
    }
}

__STATIC_INLINE
int __GET_KEY_COLUMN(void)    {

    __SELECT_COLUMN(COL0);
    DELAY_US(GPIO_DELAY);
    if (IS_ANY_KEY_PRESSED()) {
        return 0;
    }

    __SELECT_COLUMN(COL1);
    DELAY_US(GPIO_DELAY);
    if (IS_ANY_KEY_PRESSED()) {
        return 1;
    }

    __SELECT_COLUMN(COL2);
    DELAY_US(GPIO_DELAY);
    if (IS_ANY_KEY_PRESSED()) {
        return 2;
    }

    __SELECT_COLUMN(COL3);
    DELAY_US(GPIO_DELAY);
    if (IS_ANY_KEY_PRESSED()) {
        return 3;
    }

    return -1; // no key pressed
}

char fn_get_key(void)   {
    RESET_KEYPAD();     // set all columns to low, so that all rows can be read
    DELAY_US(GPIO_DELAY);

    // if no key is pressed, return 0
    if (!IS_ANY_KEY_PRESSED()) {
        return 0;
    }

    int col = __GET_KEY_COLUMN();
    int row = __GET_KEY_ROW();

    RESET_KEYPAD();         // get read for next key press
    DELAY_US(GPIO_DELAY);

    // if there was an error, return 0
    if (col == -1 || row == -1) {
        return 0;
    }

    uint8_t col_idx = (uint8_t) col & 0x03;
    uint8_t row_idx = (uint8_t) row & 0x03;

    return KEY_MAP[row_idx][col_idx];
}


#endif