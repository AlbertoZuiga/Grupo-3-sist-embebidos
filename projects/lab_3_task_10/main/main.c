/* Control de Motor con Botón

   Este código está en el Dominio Público (o con licencia CC0, a su elección).

   A menos que sea requerido por la ley aplicable o acordado por escrito, este
   software se distribuye "COMO ESTÁ", SIN GARANTÍAS O CONDICIONES DE NINGÚN TIPO, ya sea expresas o implícitas.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "example";

#define BUTTON_GPIO 0
#define IN1 4
#define IN2 2

static bool button_pressed = false;
static int motor_state = 0;

static void configure_motor(void)
{
    gpio_reset_pin(IN1);
    gpio_reset_pin(IN2);
    gpio_set_direction(IN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(IN2, GPIO_MODE_OUTPUT);
    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 0);
}

void button_task(void *pvParameter)
{
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);

    while (1) {
        if (gpio_get_level(BUTTON_GPIO) == 0 && !button_pressed) {
            button_pressed = true;
            motor_state = (motor_state + 1) % 4;

            switch (motor_state) {
                case 0: // Detener motor
                    gpio_set_level(IN1, 0);
                    gpio_set_level(IN2, 0);
                    ESP_LOGI(TAG, "Motor stopped");
                    break;
                case 1: // Girar motor en un sentido
                    gpio_set_level(IN1, 1);
                    gpio_set_level(IN2, 0);
                    ESP_LOGI(TAG, "Motor rotating forward");
                    break;
                case 2: // Detener motor
                    gpio_set_level(IN1, 0);
                    gpio_set_level(IN2, 0);
                    ESP_LOGI(TAG, "Motor stopped");
                    break;
                case 3: // Girar motor en sentido contrario
                    gpio_set_level(IN1, 0);
                    gpio_set_level(IN2, 1);
                    ESP_LOGI(TAG, "Motor rotating backward");
                    break;
            }
        } else if (gpio_get_level(BUTTON_GPIO) == 1) {
            button_pressed = false;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    configure_motor();
    xTaskCreate(&button_task, "button_task", 2048, NULL, 5, NULL);
}
