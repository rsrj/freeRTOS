#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static TaskHandle_t xReceiverHandler = NULL, xSenderHandler = NULL;

void sender(void * params)
{
    while (true)
    {
        xTaskNotify( xReceiverHandler, (1 << 0), eSetValueWithoutOverwrite );
        vTaskDelay(1000 / portTICK_RATE_MS);
        xTaskNotify( xReceiverHandler, (1 << 1), eSetValueWithoutOverwrite );
        vTaskDelay(1000 / portTICK_RATE_MS);
        xTaskNotify( xReceiverHandler, (1 << 2), eSetValueWithoutOverwrite );
        vTaskDelay(1000 / portTICK_RATE_MS);
        xTaskNotify( xReceiverHandler, (1 << 3), eSetValueWithoutOverwrite );
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void receiver(void * params)
{
    uint32_t state;
    while (true)
    {
        xTaskNotifyWait( 0, 0, &state, portMAX_DELAY); 
        printf("Received state %d \n", state);
    }
}

void app_main(void)
{
    xTaskCreate(&receiver, "receiver", 2048, NULL, 2, &xReceiverHandler);
    xTaskCreate(&sender, "sender", 2048, NULL, 2, &xSenderHandler);
}
