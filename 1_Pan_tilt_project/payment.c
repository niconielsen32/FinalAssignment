/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: payment.c
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
#include "emp_type.h"
#include "glob_def.h"
#include "payment.h"
#include "buttons.h"
#include "LCD.h"
#include "string.h"
#include "digiswitch.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/
INT16U payment_type = 3;

BOOLEAN is_payment_complete = FALSE;

INT16U que_buffer;
INT16U total_cash;
INT16U stop_payment;
BOOLEAN pulses_clockwise; // = get_digi_direction
BOOLEAN is_pin_even;
/*****************************   Functions   *******************************/


//INT16U select_payment_type(INT16U payment){
//
//    if(payment == CARD)
//        return CARD;
//    if(payment== CASH)
//        return CASH;
//}


BOOLEAN get_payment_complete(){
    return is_payment_complete;
}

void set_payment_complete(BOOLEAN payment_complete){
    is_payment_complete = payment_complete;
}


INT16U get_payment_type(){
    return payment_type;
}

void payment_task(void* pvParameters){

    while(1){



//        if (payment_type == 0 || payment_type == 1){
//        gfprintf(COM2, "%c%cPayment type is:", 0x1B, 0x80);
//        gfprintf(COM2, "%c%c     %05u", 0x1B, 0xA8, payment_type);
//        } else {
//            payment_type = get_pay_type();
//        }

        payment_type = get_pay_type();
        stop_payment = get_payment_stop();

        switch(payment_type){
              case CARD:

                  xQueuePeek(Q_KEY, que_buffer, (TickType_t) 10); //Q-key mangler

                  if(que_buffer % 2 == 0){
                      is_pin_even = TRUE;
                      is_payment_complete = TRUE;
                  } else {
                      is_pin_even = FALSE;
                      is_payment_complete = TRUE;
                  }
              break;

              case CASH:

                      if(stop_payment){
                         total_cash = get_total_cash();
                         is_payment_complete = TRUE;
                      }

              break;

        }
    }
}
/****************************** End Of Module *******************************/

