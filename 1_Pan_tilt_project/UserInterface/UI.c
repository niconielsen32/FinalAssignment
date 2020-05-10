
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
#include "UI.h"
#include "buttons.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/



void UI_task(void * pvParameters)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{
    INT8U button;


    while(1)
    {
        button = get_button_state;

        // test for buttons working

               if(!(GPIO_PORTF_DATA_R & 0x10)){ //sw1
                   //GPIO_PORTF_DATA_R = 0x08; //put it on the green LED
                   button = "button1";
               } else if(!(GPIO_PORTF_DATA_R & 0x01)){ //sw2
                   //GPIO_PORTF_DATA_R = 0x02; //put it on the RED LED
                   button = "button2";
               } else {
                   //GPIO_PORTF_DATA_R = 0x00;
                   button = "No button";
               }


//        write_string(button);
//        write_character(' ');

          write_character(button);
          write_character(' ');
//        // write output
//        // Motor 1
//        write_string(sMotor1);
//        write_string(sVel);
//        write_fp32(leadfree92);
//        write_character(' ');

//        write_cr();


        vTaskDelay(pdMS_TO_TICKS(100));
    }


}

/****************************** End Of Module *******************************/




