#ifndef _MY_LED_H_
#define _MY_LED_H_

#include "esp_err.h"
#include "hal/gpio_types.h"
#include <stdio.h>
#include "driver/gpio.h"

typedef struct {
    gpio_num_t plus_pin, minus_pin;
    bool is_on;
} my_led_t;

typedef enum {
    LED_STATE_OFF = 0,
    LED_STATE_ON
} led_state_t ;

void my_led_set_level(my_led_t led, led_state_t state);
void my_led_init(my_led_t led);



#endif