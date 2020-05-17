/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: digiswitch.c
*
* PROJECT....: EMP
*
*  Definements of variable types.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "emp_type.h"
#include "glob_def.h"
#include "payment.h"
#include "digiswitch.h"
#include "UserInterface/write.h"
/*****************************    Defines    *******************************/

#define clockwise 0
#define counter_clock_wise 1

/*****************************   Constants   *******************************/

INT16U digi_pulses = 0;
INT16U direction;

INT8U DigiA = 0;
INT8U DigiB = 0;
/*****************************   Functions   *******************************/
INT8U get_direction(){
    return direction;
}
INT16U get_digi_pulses(){
    return digi_pulses;
}

void digiswitch_task(void* pvParameters) {

    while(1){

        //if(!(GPIO_PORTF_DATA_R & 0x10))
       DigiA = (GPIO_PORTA_DATA_R & 0x20);
       DigiB = (GPIO_PORTA_DATA_R & 0x40);

       write_int16u(DigiA);
       write_string("  ");
       write_int16u(DigiB);
       write_string("  ");


    }
}
/****************************** End Of Module *******************************/

