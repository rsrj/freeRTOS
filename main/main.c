#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "esp_system.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "driver/uart.h"

typedef float (*calculator)(float, float); //Declaring a function pointer

float sum ( float a , float b )
{
    return a + b;
}

float mult ( float a , float b)
{
    return a * b;
}

float sub ( float a , float b)
{
    return a - b;
}

float division ( float a , float b)
{
    return a / b;
}

calculator selectFunction(char operationSelector)
{
    calculator operationFunction;
    switch (operationSelector)
    {
    case '+':
        operationFunction = sum;
        break;
    case '-':
        operationFunction = sub;
        break;
    case '*':
        operationFunction = mult;
        break;
    case '/':
        operationFunction = division;
        break;
    default:
        operationFunction = NULL;
        break;
    }
    return operationFunction;
}
void app_main(void)
{
    /* Initializing buffer to write in the console*/
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);
    
    calculator calc;
    char selector;
    bool proceed = true;
    do
    {
        //menu();
        printf("************CALCULATOR***********\n");
        printf("Select the operation (+ , - , / , *):\n");
        scanf("%c", &selector);
        fflush(stdin);
        calc = selectFunction(selector);
        if (calc != NULL)
        {
            float a,b;
            printf("Insert the first value: ");
            scanf("%f", &a);
            fflush(stdin);
            printf("\nInsert the second value: ");
            scanf("%f", &b);
            fflush(stdin);
            printf("\nThe result is %.2f\n", calc(a, b));
        }
        else 
        {
            printf("Invalid operation\n");
        }

        char cont;
        printf("Do you want to continue? (Y for yes, N for no)\n");
        scanf("%c", &cont);
        fflush(stdin);
        switch (cont)
        {
            case 'Y':
            case 'y':
                proceed = true;
            break;
            case 'N':
            case 'n':
                proceed = false;
            break;
            default:
                printf("Invalid option, try again\n");
            break;
        }
        
    } while (proceed);
    printf("\nCalculator finished\n");
}
