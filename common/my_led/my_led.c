#include "my_led.h"
#include "output_io.h"

void my_led_set_level(my_led_t led, led_state_t state) {
    output_io_set_level(led.plus_pin, state);
    if (led.minus_pin != GPIO_NUM_NC) output_io_set_level(led.minus_pin, !state);
}


void my_led_init(my_led_t led) {
    output_io_create(led.plus_pin);
    if (led.minus_pin != GPIO_NUM_NC) output_io_create(led.minus_pin);
}