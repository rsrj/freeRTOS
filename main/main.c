#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/** MUTEX EXAMPLE
 * 
 *  Mutual exclusion
 *  It's a type of semaphore that only the task that blocks a another task can unblock it
 *  It is helpful when one task cannot call another task until all the work had finished inside it.
 *  
 */
/*Creates a handle that will be used by tasks*/
xSemaphoreHandle mutexBus;

/*Function that will be handled and blocked until the end processing*/
void writeToBus(char * message)
{
    printf(message);
}

void task1(void * params)
{
    while (true)
    {
        printf("Reading temperature\n");
        /*Blocks the mutexBus*/
        if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
        {
            writeToBus("Temperature is 25C\n");
            /*Unblocks the mutex to be used by another task*/
            xSemaphoreGive(mutexBus);
        }
        else 
        {
            printf("Writing temperature timed out\n");
        }
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2(void * params)
{
    while (true)
    {
        printf("Reading humidity\n");
        if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
        {
            writeToBus("Humidity is 50\n");
            xSemaphoreGive(mutexBus);
        }
        else
        {
            printf("Writing humidity timed out\n");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    /*Creating the Mutex semaphore and associate it with the semaphore handler*/
    mutexBus = xSemaphoreCreateMutex();
    xTaskCreate(&task1, "temperature", 2048, NULL, 2, NULL);
    xTaskCreate(&task2, "humidity", 2048, NULL, 2, NULL);
}
