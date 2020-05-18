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
#include "string.h"
#include "UserInterface/write.h"
#include "LCD.h"
#include "file.h"
#include "keypad.h"
#include "queue.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

BOOLEAN is_payment_complete = FALSE;

INT16U payment_type;
INT8U que_buffer;
INT16U adc_value;
INT16U digi_pulses = 0;
INT16U total_cash = 0;
INT16U cash_invalid;
BOOLEAN pulses_clockwise; // = get_digi_direction
BOOLEAN is_pin_even;


/*****************************   Functions   *******************************/


//INT16U select_payment_type(INT16U payment){
//    if(payment == CARD)
//        return CARD;
//    if(payment== CASH)
//        return CASH;
//}

BOOLEAN payment_complete(){
    return is_payment_complete;
}

INT16U get_total_cash(){
    return total_cash;
}


void payment_task(void* pvParameters){


    while(1){

        //select_pay_type();
        payment_type = get_pay_type();

        //payment_type = select_payment_type(CARD);

//
//        if (payment_type == 2){
//            gfprintf(COM2, "%c%cPay Type is     ", 0x1B, 0x80);
//            gfprintf(COM2, "%c%c       %u       ", 0x1B, 0xA8, payment_type);
//            write_int16u(payment_type);
//        } else if(payment_type == 1){
//            gfprintf(COM2, "%c%cPay Type is     ", 0x1B, 0x80);
//            gfprintf(COM2, "%c%c       %u       ", 0x1B, 0xA8, payment_type);
//            write_int16u(payment_type);
//        } else {
//            payment_type = get_pay_type();
//        }



        //write_int16u(payment_type);

        cash_invalid = get_button_state();

        switch(payment_type){

              case CARD:
                  if(get_paytype_complete()){
                      //write_string("weout");
                      xQueuePeek(Q_CARD, &que_buffer, 0); //Q-key mangler
                      write_int16u(que_buffer);
                      is_payment_complete = TRUE; // KUN TIL TEST, SKAL IKKE VÆRE HER
                  }
//                  if(que_buffer % 2 == 0){
//                      is_pin_even = TRUE;
//                      is_payment_complete = TRUE;
//                  } else {
//                      is_pin_even = FALSE;
//                      is_payment_complete = TRUE;
//                  }
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
                  if(cash_invalid != 0){
                      is_payment_complete = TRUE;
                  }
                 }

              break;

        }
        //write_int16u(que_buffer);


    }
}
/****************************** End Of Module *******************************/

