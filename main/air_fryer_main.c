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
#include "soc/rtc_wdt.h"

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 2

// define touch button
#define BUTTON_PLUS     TOUCH_PAD_NUM9
#define BUTTON_MINUS    TOUCH_PAD_NUM8
// #define BUTTON_MENU     TOUCH_PAD_NUM7
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
#define LED_7_KAT1      GPIO_NUM_26
#define LED_7_KAT2      GPIO_NUM_21
#define LED_7_KAT3      GPIO_NUM_19

enum menu {
    MENU_TIME=0,
    MENU_TEMP,
    MENU_MAX
};

my_led_t leds[NUM_LED];
my_7_led_t leds_seg[3];
enum menu current_menu = MENU_TEMP;
int16_t time = 20;
int16_t temp = 200;
uint32_t change_mode_counter=0;

void show_temp() {
    if (temp > 999) temp = 999;
    my_7_led_set_digit(&leds_seg[0], temp/100);
    my_7_led_set_digit(&leds_seg[1], temp%100/10);
    my_7_led_set_digit(&leds_seg[2], temp%10);
}

void show_time(uint16_t time) {
    // if (time > 99) time=99;
    // my_7_led_set_digit(&leds_seg[0], 0);
    // my_7_led_set_digit(&leds_seg[1], time/10);
    // my_7_led_set_digit(&leds_seg[2], time%10);
}

void input_event_callback(int pin) {
    if(pin==GPIO_NUM_0) {
        // static int x = 0;
        // output_io_set_level(BLINK_GPIO, x);
        // x = 1-x;
        // output_io_toggle(BLINK_GPIO);
    }
}

void input_touch_event_callback(touch_pad_t touch_pad_num) {
    if(touch_pad_num == BUTTON_MINUS) {
        if (current_menu==MENU_TEMP) {
            temp-=5;
        }
        if (current_menu==MENU_TIME) {
            time-=5;
        }
    }
    if(touch_pad_num == BUTTON_PLUS) {
        if (current_menu==MENU_TEMP) {
            temp+=5;
        }
        if (current_menu==MENU_TIME) {
            time+=5;
        }
    }
    if(touch_pad_num==BUTTON_TIME) {
        current_menu=MENU_TIME;
    }
    if(touch_pad_num==BUTTON_TEMP) {
        current_menu=MENU_TEMP;
    }
    // ESP_LOGI("TEMP TIME", "time: %d \t, temp: %d", time, temp);
    // change_mode_counter = 1;
}


void led_matrix() {
    uint16_t speed = 10; //ms
    while(1) {
        // for menu led
        // ESP_LOGI("LED MATRIX","I'm hehe");
        output_io_set_level(GPIO_NUM_27, 0);
        for (int i = 0; i<NUM_LED; i++) {
            if (leds[i].is_on) {
                my_led_set_level(leds[i], 1);
            }
        }
        vTaskDelay(speed / portTICK_PERIOD_MS);
        for (int i = 0; i<NUM_LED; i++) {
            my_led_set_level(leds[i], 0);
        }
        output_io_set_level(GPIO_NUM_27, 1);
        // for 1st 7 segment led
        for (int i = 0; i< 3; i++) {
            if (leds_seg[i].is_on){
                output_io_set_level(leds_seg[i].Kat, 0);
                for (int j = 0; j<7; j++) {
                    if (leds_seg[i].leds[j].is_on) my_led_set_level(leds_seg[i].leds[j], LED_STATE_ON);
                    // ESP_LOGI("LED MATRIX", "leds_seg[%d].leds[%d]=%d", i, j, leds_seg[i].leds[j].is_on);
                }
                vTaskDelay(speed / portTICK_PERIOD_MS);
                for (int j = 0; j<7; j++) {
                    my_led_set_level(leds_seg[i].leds[j], LED_STATE_OFF);
                }
                output_io_set_level(leds_seg[i].Kat, 1);
            }
        }
        // for 2nd 7 segment led

        // for 3rd 7 segment led


    }
}

void change_menu() {
    while(1) {
        if (current_menu==MENU_TEMP) {
            show_temp();
        }
        if (current_menu==MENU_TIME) {
            show_time(time);
        }
        // if (change_mode_counter++ % 10 == 0) {
        //     current_menu = (current_menu+1)%MENU_MAX;
        //     // ESP_LOGI("CHANGE MENU", "Changed menu to %d", current_menu);
        // }
        // ESP_LOGI("CHANGE MENU","Checking...");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{

    output_io_create(BLINK_GPIO);

    input_io_create(GPIO_NUM_0, HI_TO_LO);
    input_set_callback(input_event_callback);

    input_touch_set_callback(input_touch_event_callback);
    input_touch_init();
    input_touch_io_create(BUTTON_PLUS, 20);
    input_touch_io_create(BUTTON_MINUS, 20);
    input_touch_io_create(BUTTON_POWER, 10);
    input_touch_io_create(BUTTON_TEMP, 10);
    input_touch_io_create(BUTTON_TIME, 15);
    

    
    leds[0] = (my_led_t)    { .plus_pin = GPIO_NUM_0,   .minus_pin= GPIO_NUM_27, .is_on=1}; // LED 4
    leds[1] = (my_led_t)    { .plus_pin = GPIO_NUM_4,   .minus_pin= GPIO_NUM_27, .is_on=1}; // LED 5
    leds[2] = (my_led_t)    { .plus_pin = GPIO_NUM_2,   .minus_pin= GPIO_NUM_27, .is_on=1}; // LED 5
    leds[3] = (my_led_t)    { .plus_pin = GPIO_NUM_16,  .minus_pin= GPIO_NUM_27, .is_on=1}; // LED 5
    leds[4] = (my_led_t)    { .plus_pin = GPIO_NUM_17,  .minus_pin= GPIO_NUM_27, .is_on=1}; // LED 4
    leds[5] = (my_led_t)    { .plus_pin = GPIO_NUM_5,   .minus_pin= GPIO_NUM_27, .is_on=1}; // LED 5
    leds[6] = (my_led_t)    { .plus_pin = GPIO_NUM_15,  .minus_pin= GPIO_NUM_27, .is_on=1}; // LED 5
    leds[7] = (my_led_t)    { .plus_pin = GPIO_NUM_18,  .minus_pin= GPIO_NUM_27, .is_on=1}; // LED 5
    

    for (int i = 0; i<NUM_LED; i++) {
        my_led_init(leds[i]);
        // if (leds[i].is_on) my_led_set_level(leds[i], LED_STATE_ON);
    }


    leds_seg[0] = my_7_led_create(LED_7_A, LED_7_B, LED_7_C, LED_7_D, LED_7_E, LED_7_F, LED_7_G, LED_7_KAT1);
    leds_seg[1] = my_7_led_create(LED_7_A, LED_7_B, LED_7_C, LED_7_D, LED_7_E, LED_7_F, LED_7_G, LED_7_KAT2);
    leds_seg[2] = my_7_led_create(LED_7_A, LED_7_B, LED_7_C, LED_7_D, LED_7_E, LED_7_F, LED_7_G, LED_7_KAT3);

    
    // my_7_led_set_digit(&leds_seg[1], 6);
    // my_7_led_set_digit(&leds_seg[2], 3);
    // my_7_led_set_state(&leds_seg[0], 0b1101100);
    // my_7_led_set_state(leds_seg[2], 0b1101111);
    xTaskCreate(&led_matrix, "led_matrix", 2048, NULL, 4, NULL);
    xTaskCreate(&change_menu, "change_menu", 2048, NULL, 3, NULL);
    // xTaskCreate(&led_7, "led_7", 2048, NULL, 5, NULL);

    show_temp();
    
}
