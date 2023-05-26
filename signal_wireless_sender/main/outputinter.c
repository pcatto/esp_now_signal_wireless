#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdio.h>

#define led 25
#define button 27

esp_err_t init_led(void);
esp_err_t init_irs(void);
void gpio_isr_handler(void* args);

const char *tag = "Main";
uint8_t count = 0;


esp_err_t init_led(){
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    ESP_LOGI(tag, "init led complete");
    return ESP_OK;
}

esp_err_t init_irs(){
    gpio_config_t pGPIOConfig;
    pGPIOConfig.pin_bit_mask = (1ULL << button);
    pGPIOConfig.mode = GPIO_MODE_DEF_INPUT;
    pGPIOConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    pGPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pGPIOConfig.intr_type = GPIO_INTR_HIGH_LEVEL;

    gpio_config(&pGPIOConfig);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(button, gpio_isr_handler, NULL);
    ESP_LOGI(tag, "init irs complete");
    return ESP_OK;
}
void gpio_isr_handle(void* args){
    ESP_LOGI(tag, "interruption call complete");
    count++;
    if(count > 2){
        count = 0;
    }

    switch(count)
    {
        case 0:
            gpio_set_level(led, 1);
            break;
        default:
            break;
    }
}