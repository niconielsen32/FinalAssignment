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


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/
INT16U payment_type;

BOOLEAN is_payment_complete = FALSE;

INT16U que_buffer;
INT16U adc_value;
INT16U digi_pulses = 0;
INT16U total_cash = 0;
INT16U cash_invalid;
BOOLEAN pulses_clockwise; // = get_digi_direction
BOOLEAN is_pin_even;
/*****************************   Functions   *******************************/


INT16U select_payment_type(INT16U payment){
    if(payment == CARD)
        return CARD;
    if(payment== CASH)
        return CASH;
}

BOOLEAN get_payment_complete(){
    return is_payment_complete;
}

void set_payment_complete(BOOLEAN payment_complete){
    is_payment_complete = payment_complete;
}

INT16U get_total_cash(){
    return total_cash;
}

INT16U get_payment_type(){
    return payment_type;
}

void payment_task(void* pvParameters){

    while(1){

        payment_type = select_payment_type(CASH);

        cash_invalid = get_button_state();

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
                  //adc_value = get_adc(); //evt fra en que, skal laves om
                  //digi_pulses = get_digi_pulses();
                  while(!is_payment_complete){
                  if(pulses_clockwise){                                        //mangler
                      total_cash += 100;
                  } else if(!pulses_clockwise){
                      total_cash += 10;
                  }
                  if(cash_invalid != idle){
                      is_payment_complete = TRUE;
                  }
                 }

              break;

        }


    }
}
/****************************** End Of Module *******************************/

