#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/*Creates a binary semaphore handler that will be used to control the semaphore*/
xSemaphoreHandle binSemaphore;

void listenForHTTP(void * params)
{
    while (true)
    {
        printf("received http message\n");
        /*When reaches the SemaphoreGive, releases the high priority task with SemaphoreTake*/
        xSemaphoreGive(binSemaphore);
        printf("processed http message\n");
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}

void task1(void * params)
{
    while (true)
    {
        xSemaphoreTake(binSemaphore, portMAX_DELAY);
        printf("Doing something with http in task1\n");
    }
}

void app_main(void)
{
    binSemaphore = xSemaphoreCreateBinary();
    /*If the tasks have the same priority, once the semaphoreGive method is reached, 
    it jumps to task1 (which has the semaphoreTake method) and then returns to listenForHTTP. 
    Otherwise, if the listenForHTTP function has higher priority, the entire task will run 
    until it is put to sleep.
    In the first case, output is : "received http message", "Doing something with http in task1" 
    and "processed http message". In the second case, output is: "received http message", "processed http message" 
    and "Doing something with http in task1"*/
    xTaskCreate(&listenForHTTP, "get http", 2048, NULL, 2, NULL);
    xTaskCreate(&task1, "do something with http", 2048, NULL, 2, NULL);
}
