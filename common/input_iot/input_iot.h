#ifndef INPUT_IO_H
#define INPUT_IO_H

#include "esp_err.h"
#include "hal/gpio_types.h"
#include <stdio.h>
#include "driver/gpio.h"

typedef void input_callback_t(int);

typedef enum {
    LO_TO_HI = 1,
    HI_TO_LO,
    ANY_EDGE
} interrupt_type_edge_t;

void input_io_create(gpio_num_t gpio_num, interrupt_type_edge_t type);
int input_io_get_level(gpio_num_t gpio_num);
void input_set_callback(void *cb);


#endif