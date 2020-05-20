/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: debugger.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "glob_def.h"
#include "uart0.h"
#include "write.h"

/*****************************    Defines    *******************************/
#define QUEUE_LENGTH 256


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
QueueHandle_t write_queue = NULL;


/*****************************   Functions   *******************************/

void write_character(INT8U character)
{
    xQueueSendToBack( write_queue, &character, (TickType_t) 0);
}

void write_string(INT8U* string)
{
    while( *string ) // while stringpointer is not null
    {
        INT8U character = *string++; // increments string pointer
        xQueueSendToBack( write_queue, &character, (TickType_t) 0);
    }
}

void write_int16u(INT16U number)
{
    INT8U output[5];

    output[0] = number/10000;               // gets most significant digit
    output[1] = (number%10000)/1000;
    output[2] = (number%1000)/100;
    output[3] = (number%100)/10;
    output[4] = (number%10)/1;              // gets least significant digit

    BOOLEAN start_writing_zeroes = 0;
    for(INT8U i = 0; i < 5; i++)
    {
        if( output[i] != 0 )                // if number doesn't contain digit make sure it doesn't output zeroes before first non zero digits ex. 01200
            start_writing_zeroes = 1;
        if( output[i] != 0 || start_writing_zeroes == 1 )
            write_character(output[i] + '0');
        else {
            write_character(' ');
        }
        if (i == 3)
            start_writing_zeroes = 1;
    }
}

void write_int16s(INT16S number)
{
    INT16U number_u = number;
    if (number < 0)
    {
        write_character('-');
        number_u = -number_u;
    }
    else {
       write_character(' ');
    }

    INT8U output[5];

    output[0] = number_u/10000;               // gets most significant digit
    output[1] = (number_u%10000)/1000;
    output[2] = (number_u%1000)/100;
    output[3] = (number_u%100)/10;
    output[4] = (number_u%10)/1;              // gets least significant digit

    BOOLEAN start_writing_zeroes = 0;
    for(INT8U i = 0; i < 5; i++)
    {
        if( output[i] != 0 )                // if number doesn't contain digit make sure it doesn't output zeroes before first non zero digits ex. 01200
            start_writing_zeroes = 1;
        if( output[i] != 0 || start_writing_zeroes == 1 )
            write_character(output[i] + '0');
        else {
               write_character(' ');
        }
        if (i == 3)
               start_writing_zeroes = 1;
    }
}

void write_cr()
{
    write_character(13);
}

void write_fp32(FP32 number)
{
    INT16S number_int = (INT16S) (number*1000);
    INT16U number_u = number_int;
    if (number < 0.0f)
    {
        write_character('-');
        number_u = -number_u;
    }
    else {
       write_character(' ');
    }


    INT8U output[5];

    output[0] = number_u/10000;               // gets most significant digit
    output[1] = (number_u%10000)/1000;
    output[2] = (number_u%1000)/100;
    output[3] = (number_u%100)/10;
    output[4] = (number_u%10)/1;              // gets least significant digit

    BOOLEAN start_writing_zeroes = 0;
    for(INT8U i = 0; i < 5; i++)
    {
        if( output[i] != 0 ||  start_writing_zeroes == 1 )
            write_character(output[i] + '0');
        else {
            write_character(' ');
        }
        if( i == 1 )
            write_character('.');
        start_writing_zeroes = 1;
    }
}

void write_fp32_one_digit(FP32 number)
{
    INT16S number_int = (INT16S) (number*1000);
    INT16U number_u = number_int;
    if (number < 0.0f)
    {
        write_character('-');
        number_u = -number_u;
    }
    else {
       write_character(' ');
    }


    INT8U output[5];

    output[0] = number_u/10000;               // gets most significant digit
    output[1] = (number_u%10000)/1000;
    output[2] = (number_u%1000)/100;
    output[3] = (number_u%100)/10;
    output[4] = (number_u%10)/1;              // gets least significant digit

    BOOLEAN start_writing_zeroes = 0;
    for(INT8U i = 0; i < 5; i++)
    {
        if( output[i] != 0 ||  start_writing_zeroes == 1 )
            write_character(output[i] + '0');
        else {
            write_character(' ');
        }
        if( i == 3 )
            write_character('.');
        start_writing_zeroes = 1;
    }
}


void init_write()
{
    write_queue = xQueueCreate( QUEUE_LENGTH, sizeof(INT8U) );
    uart0_init(115200,8,1,0);
}

void write_task(void *pvParameters)
{
    INT8U character;
    while(1)
    {
        if( xQueueReceive(write_queue, &character, (TickType_t) 0) )
        {
            uart0_putc(character);
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }
}


/****************************** End Of Module *******************************/
