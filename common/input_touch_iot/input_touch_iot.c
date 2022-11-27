#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"
#include "soc/sens_periph.h"
#include "input_touch_iot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define TOUCH_THRESH_NO_USE   (0)
#define TOUCH_THRESH_PERCENT  (80)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

input_touch_callback_t *input_touch_callback = NULL;
bool touch_init = false;
static const char *TAG = "Touch pad";
static bool s_pad_activated[TOUCH_PAD_MAX];
static bool s_pad_enabled[TOUCH_PAD_MAX];
static uint32_t s_pad_threshold[TOUCH_PAD_MAX];

static void touch_input_handler(void* arg) {
    uint32_t pad_intr = touch_pad_get_status();
    //clear interrupt
    touch_pad_clear_status();
    for (int i = 0; i < TOUCH_PAD_MAX; i++) {
        if (((pad_intr >> i) & 0x01)) {
            s_pad_activated[i] = true;
        }
    }
}

static void set_thresholds(touch_pad_t touch_pad_num, uint16_t value) {
    uint32_t init_value = 0;
    uint32_t sum_init_value = 0;
    uint32_t num_sample = 100;
    
    uint32_t i = num_sample;
    while(i) {
        touch_pad_read_filtered(touch_pad_num, &init_value);
        if (init_value > 0) {
            sum_init_value += init_value;
            i--;
        }
        // ESP_LOGI(TAG, "test init: touch pad [%d] val is %d", touch_pad_num, init_value);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    uint32_t threshold_val = sum_init_value/num_sample-25;
    ESP_LOGI(TAG, "Init completed: touch pad [%d] threshold val is %d", touch_pad_num, threshold_val);
    s_pad_threshold[touch_pad_num] = threshold_val;
}

static void tp_example_read_task(void *pvParameter)
{
    while (1) {
        for (int i = 0; i < TOUCH_PAD_MAX; i++) {
            if (s_pad_enabled[i]) {
                uint16_t value = 0;
                touch_pad_read_filtered(i, &value);

                if (value <= s_pad_threshold[i]) {
                    ESP_LOGI(TAG, "T%d activated! Value: %d", i, value);
                    input_touch_callback(i);
                    // Wait a while for the pad being released
                    vTaskDelay(400 / portTICK_PERIOD_MS);
                }
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void input_touch_init() {
    ESP_LOGI(TAG, "Initializing touch pad");
    ESP_ERROR_CHECK(touch_pad_init());
    // Set reference voltage for charging/discharging
    // For most usage scenarios, we recommend using the following combination:
    // the high reference valtage will be 2.7V - 1V = 1.7V, The low reference voltage will be 0.5V.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    // Initialize and start a software filter to detect slight change of capacitance.
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
    // Start a task to show what pads have been touched
    xTaskCreate(&tp_example_read_task, "touch_pad_read_task", 2048, NULL, 5, NULL);
}

void input_touch_io_create(touch_pad_t touch_pad_num, uint16_t threshold_value) {
    // Init touch pad IO
    touch_pad_config(touch_pad_num, TOUCH_THRESH_NO_USE);
    set_thresholds(touch_pad_num, threshold_value);
    s_pad_enabled[touch_pad_num] = true;
}


void input_touch_set_callback(void *cb) {
    input_touch_callback = cb;
}