#ifndef _MY_7_LED_H_
#define _MY_7_LED_H_

#include "esp_err.h"
#include "hal/gpio_types.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "my_led.h"

typedef struct {
    gpio_num_t Kat;
    my_led_t leds[7];
    bool is_on;
} my_7_led_t;


void my_7_led_set_digit(my_7_led_t *led, uint16_t digit);
void my_7_led_set_state(my_7_led_t *led, uint8_t state);
void my_7_led_init(my_7_led_t led);

my_7_led_t my_7_led_create(gpio_num_t A, gpio_num_t B, gpio_num_t C, 
                    gpio_num_t D, gpio_num_t E, gpio_num_t F, 
                    gpio_num_t G, gpio_num_t Kat);



#endif