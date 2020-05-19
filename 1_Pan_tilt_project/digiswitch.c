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
//#include "digiswitch.h"
#include "UserInterface/write.h"
/*****************************    Defines    *******************************/

#define clockwise 0
#define counter_clock_wise 1

/*****************************   Constants   *******************************/

INT8U DigiA = 0;
INT8U DigiB = 0;
INT8U lastA = 0;
INT8U A = 0;
INT8U B = 0;

INT16U total_cash_digi = 0;

/*****************************   Functions   *******************************/

INT16U get_total_cash_from_digi(){
    return total_cash_digi;
}

void digiswitch_task(void* pvParameters) {



    while(1){

        DigiA = (GPIO_PORTA_DATA_R & 0x20);
        DigiB = (GPIO_PORTA_DATA_R & 0x40);

        if(DigiA == 32){
            A = 1;
        } else{
            A = 0;
        }

        if(DigiB == 64){
            B = 1;
        } else{
            B = 0;
        }

       if(A != lastA){
           if(B != A){
               total_cash_digi += 100;
           } else{
               total_cash_digi += 10;
           }
           //write_int16u(total_cash);
           //write_string("  ");
       }
       lastA = A;
    }
    vTaskDelay(pdMS_TO_TICKS(500));
}
/****************************** End Of Module *******************************/

