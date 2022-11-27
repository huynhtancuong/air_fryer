/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "input_iot.h"
#include "input_touch_iot.h"
#include "my_led.h"
#include "output_io.h"

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 2

// define touch button
#define BUTTON_PLUS TOUCH_PAD_NUM9
#define BUTTON_MINUS TOUCH_PAD_NUM8

// define led
#define MINUS_LED_1 GPIO_NUM_25
#define MINUS_LED_2 GPIO_NUM_26
#define LED_4 GPIO_NUM_2
#define LED_5 GPIO_NUM_15
#define NUM_LED 16

my_led_t leds[NUM_LED];

void input_event_callback(int pin) {
    if(pin==GPIO_NUM_0) {
        // static int x = 0;
        // output_io_set_level(BLINK_GPIO, x);
        // x = 1-x;
        output_io_toggle(BLINK_GPIO);
    }
}

void input_touch_event_callback(touch_pad_t touch_pad_num) {
    if(touch_pad_num == BUTTON_MINUS) {
        output_io_toggle(LED_5);
    }
    if(touch_pad_num == BUTTON_PLUS) {
        output_io_toggle(LED_4);
    }
}

void led_matrix() {
    uint32_t multiplexing_speed = 10;
    while (1) {
        for (int i = 0; i< NUM_LED; i++) {
            if (leds[i].is_on){
                my_led_set_level(leds[i], LED_STATE_ON);
                vTaskDelay(multiplexing_speed / portTICK_PERIOD_MS);
                my_led_set_level(leds[i], LED_STATE_OFF);
            }
            else {
                my_led_set_level(leds[i], LED_STATE_OFF);
            }
        }
    }
}

void app_main(void)
{

    output_io_create(BLINK_GPIO);

    input_io_create(GPIO_NUM_0, HI_TO_LO);
    input_set_callback(input_event_callback);

    input_touch_set_callback(input_touch_event_callback);
    input_touch_init();
    input_touch_io_create(BUTTON_PLUS, 725);
    // input_touch_io_create(BUTTON_MINUS, 445);

    
    leds[0] = (my_led_t) { .plus_pin = GPIO_NUM_0, .minus_pin= GPIO_NUM_25, .is_on=1}; // LED 4
    leds[1] = (my_led_t) { .plus_pin = GPIO_NUM_4, .minus_pin= GPIO_NUM_25, .is_on=1}; // LED 5
    leds[2] = (my_led_t) { .plus_pin = GPIO_NUM_2, .minus_pin= GPIO_NUM_25, .is_on=1}; // LED 5
    leds[3] = (my_led_t) { .plus_pin = GPIO_NUM_16, .minus_pin= GPIO_NUM_25, .is_on=1}; // LED 5
    leds[4] = (my_led_t) { .plus_pin = GPIO_NUM_17, .minus_pin= GPIO_NUM_25, .is_on=1}; // LED 4
    leds[5] = (my_led_t) { .plus_pin = GPIO_NUM_5, .minus_pin= GPIO_NUM_25, .is_on=0}; // LED 5
    leds[6] = (my_led_t) { .plus_pin = GPIO_NUM_15, .minus_pin= GPIO_NUM_25, .is_on=0}; // LED 5
    leds[7] = (my_led_t) { .plus_pin = GPIO_NUM_18, .minus_pin= GPIO_NUM_25, .is_on=0}; // LED 5

    leds[8] = (my_led_t) { .plus_pin = GPIO_NUM_0, .minus_pin= GPIO_NUM_26, .is_on=0}; // LED 4
    leds[9] = (my_led_t) { .plus_pin = GPIO_NUM_4, .minus_pin= GPIO_NUM_26, .is_on=0}; // LED 5
    leds[10] = (my_led_t) { .plus_pin = GPIO_NUM_2, .minus_pin= GPIO_NUM_26, .is_on=0}; // LED 5
    leds[11] = (my_led_t) { .plus_pin = GPIO_NUM_16, .minus_pin= GPIO_NUM_26, .is_on=0}; // LED 5
    leds[12] = (my_led_t) { .plus_pin = GPIO_NUM_17, .minus_pin= GPIO_NUM_26, .is_on=0}; // LED 4
    leds[13] = (my_led_t) { .plus_pin = GPIO_NUM_5, .minus_pin= GPIO_NUM_26, .is_on=0}; // LED 5
    leds[14] = (my_led_t) { .plus_pin = GPIO_NUM_15, .minus_pin= GPIO_NUM_26, .is_on=0}; // LED 5
    leds[15] = (my_led_t) { .plus_pin = GPIO_NUM_18, .minus_pin= GPIO_NUM_26, .is_on=1}; // LED 5
    

    for (int i = 0; i<NUM_LED; i++) {
        my_led_init(leds[i]);
    }

    xTaskCreate(&led_matrix, "led_matrix", 2048, NULL, 5, NULL);
    
}
