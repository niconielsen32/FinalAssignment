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
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "payment.h"
#include "digiswitch.h"
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

BOOLEAN digi_complete;

char digiCash[7];

/*****************************   Functions   *******************************/

BOOLEAN get_digi_complete(){
    return digi_complete;
}

void set_digi_complete(BOOLEAN digi){
    digi_complete = digi;
}

void set_total_cash_from_digi(INT8U reset_digi){
    total_cash_digi = reset_digi;
}

INT16U get_total_cash_from_digi(){
    return total_cash_digi;
}

void digiswitch_task(void* pvParameters) {



    while(1){

        if(get_paytype_complete() && get_payment_type() == CASH && !digi_complete){

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
               write_int16u(total_cash_digi);
               write_string("  ");
               itoa(total_cash_digi, digiCash, 10);
               gfprintf(COM2, "%c%c   Total Cash    ", 0x1B, 0x80);
               gfprintf(COM2, "%c%c      %s         ", 0x1B, 0xA8, digiCash);
           }
           lastA = A;
        }
    }


    vTaskDelay(pdMS_TO_TICKS(500));
}
/****************************** End Of Module *******************************/

