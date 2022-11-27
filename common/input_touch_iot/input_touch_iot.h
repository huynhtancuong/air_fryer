#ifndef INPUT_TOUCH_IO_H
#define INPUT_TOUCH_IO_H

#include "esp_err.h"
#include "hal/gpio_types.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"
#include "soc/sens_periph.h"

typedef void input_touch_callback_t(touch_pad_t);


void input_touch_io_create(touch_pad_t touch_pad_num, uint16_t threshold_value);
void input_touch_set_callback(void *cb);
void input_touch_init();

#endif