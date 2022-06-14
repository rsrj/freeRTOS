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
        bool clear = false;
        /*This line will be blocked until the two BITS gotHttp and gotBLE are set to HIGH, 
        the flag xWaitForAllBits is set to true*/
        EventBits_t bitsReceived = xEventGroupWaitBits(evtGrp, gotHttp | gotBLE, clear, true, portMAX_DELAY);
        /*If xClearOnExit defined to true the Event_Bits after xEventGroupWaitBits must be cleared, althrough the 
        bitsReceived contains the bits before clearing*/
        EventBits_t bitsAfterWaitBits = xEventGroupGetBits(evtGrp);
        printf("\nBits received were 0x%x, and bits expected were 0x%x.\n After xEventGroupWaitBits, the bits are 0x%x, because xClearBitsOnExit was set to %s", 
        bitsReceived, gotHttp | gotBLE, bitsAfterWaitBits, (clear)? "TRUE": "FALSE\n");
        printf("\nReceived HTTP and BLE\n");

        /*Ensures that the bits were cleared after execution*/
        xEventGroupClearBits(evtGrp, gotHttp | gotBLE);
        
    }
}

void app_main(void)
{
    evtGrp = xEventGroupCreate();
    xTaskCreate(&listenForHTTP, "listenForHTTP", 2048, NULL, 1, NULL);
    xTaskCreate(&listenForBluetooth, "listenForBluetooth", 2048, NULL, 1, NULL);
    xTaskCreate(&task1, "HTTP and BLE event", 2048, NULL, 1, NULL);
}
