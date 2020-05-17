/*
 * analog.c
 *
 *  Created on: 30 Apr 2020
 *
 */
#include "analog.h"
#include "glob_def.h"
#include "ADC.h"

void ai_task()
{
    while (1)
    {
        if( xSemaphoreTake( xMutex, portMAX_DELAY ))    // protect the shared variable ai_value with a mutex
        {
//            ai_value = get_adc();                       // get the value from the adc
            xSemaphoreGive(xMutex);
        }

    }
}

