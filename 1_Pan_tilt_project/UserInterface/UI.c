
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
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include "glob_def.h"
#include "FreeRTOS.h"
#include "write.h"
#include "data.h"

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
    INT8U* button = "No button";
    while(1)
    {

        int dummy;
           SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF; // 1) activate clock for Port F
           dummy = SYSCTL_RCGC2_R;           // allow time for clock to start
           GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
           GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
           GPIO_PORTF_DIR_R = 0x0E;
           GPIO_PORTF_DEN_R = 0x1F;
           GPIO_PORTF_PUR_R = 0x11;


               if(!(GPIO_PORTF_DATA_R & 0x10)){ //sw1
                   GPIO_PORTF_DATA_R = 0x08; //put it on the green LED
                   button = "button1";
               } else if(!(GPIO_PORTF_DATA_R & 0x01)){ //sw2
                   GPIO_PORTF_DATA_R = 0x02; //put it on the RED LED
                   button = "button2";
               } else {
                   GPIO_PORTF_DATA_R = 0x00;
                   button = "No button";
               }


        write_string(button);
        write_character(' ');

//        // write output
//        // Motor 1
//        write_string(sMotor1);
//        write_string(sVel);
//        write_fp32(leadfree92);
//        write_character(' ');
//
//        write_string(sRef);
//        write_fp32(reference_motor1);
//        write_character(' ');
//
//        write_string(sDuty);
//        write_int16u(duty_cycle_motor1);
//        write_character(' ');
//
//        // Motor 2
//        write_string(sMotor2);
//        write_string(sVel);
//        write_fp32(leadfree92);
//        write_character(' ');
//
//        write_string(sRef);
//        write_fp32(reference_motor1);
//        write_character(' ');
//
//        write_string(sDuty);
//        write_int16u(duty_cycle_motor1);
//        write_character(' ');
//
//        write_cr();


        vTaskDelay(pdMS_TO_TICKS(100));
    }


}

/****************************** End Of Module *******************************/




