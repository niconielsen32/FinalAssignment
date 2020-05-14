/*
 * scale.c
 *
 *  Created on: 8 Apr 2020
 *      Author: Andreas
 */
#include "FreeRTOS.h"
#include "glob_def.h"
#include "emp_type.h"
#include "tm4c123gh6pm.h"
#include "scale.h"
#include "semphr.h"
#include "adc.h"
#include "string.h"
#include "file.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void scale_task()
{
    while(1)
    {
        if(xSemaphoreTake(xMutex, portMAX_DELAY))           // protect shared variable with a mutex
        {
            adjusted_value = ai_value * scale + offset;     // adjust the value from the adc with the scale and offset values
            xSemaphoreGive(xMutex);
        }
    }
};


/****************************** End Of Module *******************************/




