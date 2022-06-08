#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

EventGroupHandle_t evtGrp;
const int gotHttp = BIT0; //(1<<0)
const int gotBLE = BIT1; //(1<<1)

void listenForHTTP(void * params)
{
    while (true)
    {
        printf("Got HTTP\n");
        /*Set the bit gotHttp to HIGH in that evtGroup*/
        xEventGroupSetBits(evtGrp, gotHttp);
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

void listenForBluetooth(void * params)
{
    while (true)
    {
        printf("Got BLE\n");
        /*Set the bit gotBLE to HIGH in that evtGroup*/
        xEventGroupSetBits(evtGrp, gotBLE);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void task1(void *params)
{
    while(true)
    {
        /*This line will be blocked until the two BITS gotHttp and gotBLE are set to HIGH, 
        the flag xWaitForAllBits is set to true*/
        xEventGroupWaitBits(evtGrp, gotHttp | gotBLE, true, true, portMAX_DELAY);
        printf("\nReceived HTTP and BLE\n");
    }
}

void app_main(void)
{
    evtGrp = xEventGroupCreate();
    xTaskCreate(&listenForHTTP, "listenForHTTP", 2048, NULL, 1, NULL);
    xTaskCreate(&listenForBluetooth, "listenForBluetooth", 2048, NULL, 1, NULL);
    xTaskCreate(&task1, "HTTP and BLE event", 2048, NULL, 1, NULL);
}
