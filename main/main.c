#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static TaskHandle_t xReceiverHandler = NULL, xSenderHandler = NULL;

void sender(void * params)
{
    while (true)
    {
        xTaskNotifyGive(xReceiverHandler); /*Send notification to the receiver() task, bringing it out of the blocked state*/
        xTaskNotifyGive(xReceiverHandler);
        xTaskNotifyGive(xReceiverHandler);
        xTaskNotifyGive(xReceiverHandler);
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}

void receiver(void * params)
{
    while (true)
    {

        /*If xClearCountOnExit is pdTRUE it executes only once and clears the counter, 
        otherwise, if pdFALSE it executes and decrements the counter until it reaches zero.*/
        uint32_t count = ulTaskNotifyTake( pdFALSE, portMAX_DELAY ); 
        printf("Received notification %d times\n", count);
    }
}

void app_main(void)
{
    xTaskCreate(&receiver, "receiver", 2048, NULL, 2, &xReceiverHandler);
    xTaskCreate(&sender, "sender", 2048, NULL, 2, &xSenderHandler);
}
