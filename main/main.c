#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

xQueueHandle queue;

void listenForHttp(void * params)
{
    int count = 0;
    while (true)
    {
        count++;
        printf("received http message\n");
        long ok = xQueueSend(queue, &count, 1000 / portTICK_PERIOD_MS);
        if (ok)
        {
            printf("Added message to queue\n");
        }
        else
        {
            printf("failed to add message to queue\n");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task1(void * params)
{
    while (true)
    {
        int rxInt;
        if(xQueueReceive(queue, &rxInt, 5000 / portTICK_PERIOD_MS))
        {
            printf("Integer received: %d\n", rxInt);
        }
    }
}

void app_main(void)
{
    /*To create a queue is necessary to specify the number of elements and the queue size,
    once created, if the queue is full, may be not possible to put others elements until
    the data is treated*/
    queue = xQueueCreate(3, sizeof(int));
    xTaskCreate(&listenForHttp, "get http", 2048, NULL, 2, NULL);
    xTaskCreate(&task1, "do something with http", 2048, NULL, 2, NULL);
}
