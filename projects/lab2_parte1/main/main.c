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
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "example";

#define BLINK_GPIO CONFIG_BLINK_GPIO
#define BUTTON_GPIO 0

int hello_period = 1000;
int blink_period = 1000;
bool button_pressed = false;

static uint8_t s_led_state = 0;

#ifdef  CONFIG_BLINK_LED_GPIO

static void configure_led(void)
{
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

#else

#error "unsupported LED type"

#endif

void hello_task(void *pvParameter)
{
    while(1)
    {
        printf("This is Group 3!\n");
        vTaskDelay(hello_period / portTICK_PERIOD_MS);
    }
}

void blinky(void *pvParameter)
{
    while (1) {
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        gpio_set_level(BLINK_GPIO, s_led_state);
        s_led_state = !s_led_state;
        vTaskDelay(blink_period / portTICK_PERIOD_MS);
    }
}

void button_task(void *pvParameter)
{
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);

    while (1) {
        if (gpio_get_level(BUTTON_GPIO) == 0 && !button_pressed) {
            button_pressed = true;

            if (blink_period == 100) {
                blink_period = 500;
            }
            else if (blink_period == 500) {
                blink_period = 1000;
            }
            else if (blink_period == 1000) {
                blink_period = 5000;
            }
            else if (blink_period == 5000) {
                blink_period = 100;
            }
            printf("El botón está presionado");
        }
        else if (gpio_get_level(BUTTON_GPIO) == 1) {
            button_pressed = false;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    configure_led();
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky, "blinky", 2048, NULL, 5, NULL);
    xTaskCreate(&button_task, "button_task", 2048, NULL, 5, NULL);
}