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
#include "my_7_led.h"

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 2

// define touch button
#define BUTTON_PLUS     TOUCH_PAD_NUM9
#define BUTTON_MINUS    TOUCH_PAD_NUM8
#define BUTTON_MENU     TOUCH_PAD_NUM7
#define BUTTON_TIME     TOUCH_PAD_NUM5
#define BUTTON_TEMP     TOUCH_PAD_NUM6
#define BUTTON_POWER    TOUCH_PAD_NUM4

// define led
#define NUM_LED         8

#define LED_7_A         GPIO_NUM_16
#define LED_7_B         GPIO_NUM_2
#define LED_7_C         GPIO_NUM_0
#define LED_7_D         GPIO_NUM_4
#define LED_7_E         GPIO_NUM_17
#define LED_7_F         GPIO_NUM_5
#define LED_7_G         GPIO_NUM_18
#define LED_7_KAT1      GPIO_NUM_35
#define LED_7_KAT2      GPIO_NUM_21
#define LED_7_KAT3      GPIO_NUM_19

my_led_t leds[NUM_LED];
my_7_led_t leds_seg[3];

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
        my_led_set_level(leds[7], LED_STATE_ON);
    }
    if(touch_pad_num == BUTTON_PLUS) {
        my_led_set_level(leds[8], LED_STATE_ON);
    }
}

void led_matrix() {
    uint32_t multiplexing_speed = 10;
    while (1) {
        for (int i = 0; i< NUM_LED; i++) {
            if ((leds[i].is_on) && (leds[i].minus_pin == GPIO_NUM_25)){
                my_led_set_level(leds[i], LED_STATE_ON);
            }
        }
        vTaskDelay(multiplexing_speed / portTICK_PERIOD_MS);
        for (int i = 0; i< NUM_LED; i++) {
            if ((leds[i].is_on) && (leds[i].minus_pin == GPIO_NUM_25)){
                my_led_set_level(leds[i], LED_STATE_OFF);
            }
        }
    }
}

void led_7() {

}

void app_main(void)
{

    output_io_create(BLINK_GPIO);

    input_io_create(GPIO_NUM_0, HI_TO_LO);
    input_set_callback(input_event_callback);

    input_touch_set_callback(input_touch_event_callback);
    input_touch_init();
    // input_touch_io_create(BUTTON_PLUS, 20);
    // input_touch_io_create(BUTTON_MINUS, 20);
    // input_touch_io_create(BUTTON_MENU, 20);
    // input_touch_io_create(BUTTON_POWER, 10);
    // input_touch_io_create(BUTTON_TEMP, 10);
    // input_touch_io_create(BUTTON_TIME, 15);
    

    
    leds[0] = (my_led_t)    { .plus_pin = GPIO_NUM_0,   .minus_pin= NULL, .is_on=1}; // LED 4
    leds[1] = (my_led_t)    { .plus_pin = GPIO_NUM_4,   .minus_pin= NULL, .is_on=1}; // LED 5
    leds[2] = (my_led_t)    { .plus_pin = GPIO_NUM_2,   .minus_pin= NULL, .is_on=1}; // LED 5
    leds[3] = (my_led_t)    { .plus_pin = GPIO_NUM_16,  .minus_pin= NULL, .is_on=1}; // LED 5
    leds[4] = (my_led_t)    { .plus_pin = GPIO_NUM_17,  .minus_pin= NULL, .is_on=1}; // LED 4
    leds[5] = (my_led_t)    { .plus_pin = GPIO_NUM_5,   .minus_pin= NULL, .is_on=0}; // LED 5
    leds[6] = (my_led_t)    { .plus_pin = GPIO_NUM_15,  .minus_pin= NULL, .is_on=1}; // LED 5
    leds[7] = (my_led_t)    { .plus_pin = GPIO_NUM_18,  .minus_pin= NULL, .is_on=1}; // LED 5
    

    for (int i = 0; i<NUM_LED; i++) {
        my_led_init(leds[i]);
    }


    // leds_seg[0] = (my_7_led_t) { .A=LED_7_A, .B=LED_7_B, .C=LED_7_C, .D=LED_7_D, .E=LED_7_E, .F=LED_7_F, .G=LED_7_G, .Kat=LED_7_KAT1};
    // leds_seg[1] = (my_7_led_t) { .A=LED_7_A, .B=LED_7_B, .C=LED_7_C, .D=LED_7_D, .E=LED_7_E, .F=LED_7_F, .G=LED_7_G, .Kat=LED_7_KAT2};
    // leds_seg[2] = (my_7_led_t) { .A=LED_7_A, .B=LED_7_B, .C=LED_7_C, .D=LED_7_D, .E=LED_7_E, .F=LED_7_F, .G=LED_7_G, .Kat=LED_7_KAT3};

    // my_7_led_set_state(leds_seg[0], 0b1111000);

    xTaskCreate(&led_matrix, "led_matrix", 2048, NULL, 5, NULL);
    // xTaskCreate(&led_7, "led_7", 2048, NULL, 5, NULL);
    
}
