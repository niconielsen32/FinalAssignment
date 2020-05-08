
/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: gpio.c
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
#include "emp_type.h"
#include "spi.h"
#include "PID.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include "glob_def.h"
#include "FreeRTOS.h"
#include "write.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

#define TO_RAD_PER_SECONDS_SCALE 0.0174532925f

/*****************************   Functions   *******************************/



void UI_task(void * pvParameters)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{
    INT16U spi_outgoing;
    INT16U duty_cycle_motor1;
    INT16U duty_cycle_motor2;
    FP32 reference_motor1;
    FP32 reference_motor2;
    FP32 velocity;
    INT16S TEMP;

    INT8U* sVel = "v: ";
    INT8U* sRef = "r: ";
    INT8U* sDuty = "d: ";
    INT8U* sMotor1 = "M1:  ";
    INT8U* sMotor2 = " |  M2:  ";
    INT8U* sClear = "                                         ";
    write_string(sClear);
    while(1)
    {



        // write output
        // Motor 1
        write_string(sMotor1);
        write_string(sVel);
        write_fp32(velocity);
        write_character(' ');

        write_string(sRef);
        write_fp32(reference_motor1);
        write_character(' ');

        write_string(sDuty);
        write_int16u(duty_cycle_motor1);
        write_character(' ');

        // Motor 2
        write_string(sMotor2);
        write_string(sVel);
        write_fp32(velocity);
        write_character(' ');

        write_string(sRef);
        write_fp32(reference_motor1);
        write_character(' ');

        write_string(sDuty);
        write_int16u(duty_cycle_motor1);
        write_character(' ');

        write_cr();


        vTaskDelay(pdMS_TO_TICKS(500));
    }


}

/****************************** End Of Module *******************************/




