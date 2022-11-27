#include "my_7_led.h"
#include "output_io.h"
#include "esp_err.h"
#include "hal/gpio_types.h"
#include <stdio.h>
#include "driver/gpio.h"

void my_7_led_set_digit(my_7_led_t *led, uint16_t digit) {
    switch (digit)
    {
    case 0:
        my_7_led_set_state(led, 0b0111111);
        break;
    case 1:
        my_7_led_set_state(led, 0b0000110);
        break;
    case 2:
        my_7_led_set_state(led, 0b1011011);
        break;
    case 3:
        my_7_led_set_state(led, 0b1001111);
        break;
    case 4:
        my_7_led_set_state(led, 0b1100110);
        break;
    case 5:
        my_7_led_set_state(led, 0b1101101);
        break;
    case 6:
        my_7_led_set_state(led, 0b1111101);
        break;
    case 7:
        my_7_led_set_state(led, 0b0000111);
        break;
    case 8:
        my_7_led_set_state(led, 0b1111111);
        break;
    case 9:
        my_7_led_set_state(led, 0b1101111);
        break;
    
    default:
        break;
    }

}

void my_7_led_set_state(my_7_led_t *led, uint8_t state) {
    uint8_t tmp = state;
    led->is_on = true;
    for (int i = 0; i<7; i++) {
        bool led_state = tmp&1;
        tmp = tmp >> 1;
        led->leds[i].is_on = led_state;
    }
    
}

my_7_led_t my_7_led_create(gpio_num_t A, gpio_num_t B, gpio_num_t C, 
                    gpio_num_t D, gpio_num_t E, gpio_num_t F, 
                    gpio_num_t G, gpio_num_t Kat) {

    my_7_led_t object;
    object.leds[0].plus_pin = A;
    object.leds[1].plus_pin = B;
    object.leds[2].plus_pin = C;
    object.leds[3].plus_pin = D;
    object.leds[4].plus_pin = E;
    object.leds[5].plus_pin = F;
    object.leds[6].plus_pin = G;
    for (int i = 0; i<7 ;i++) {
        object.leds[i].minus_pin=Kat;
        object.leds[i].is_on = false;
    }
    object.Kat = Kat;
    output_io_create(Kat);
    output_io_set_level(Kat, 1);
    object.is_on=false;
    return object;
    

}