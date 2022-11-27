#include <stdio.h>
#include "driver/gpio.h"
#include "output_io.h"


void output_io_create(gpio_num_t gpio_num) {
    gpio_pad_select_gpio(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT_OUTPUT);
}
void output_io_set_level(gpio_num_t gpio_num, uint32_t level) {
    gpio_set_level(gpio_num, level);
}
void output_io_toggle(gpio_num_t gpio_num) {
    output_io_set_level(gpio_num, 1-gpio_get_level(gpio_num));
}