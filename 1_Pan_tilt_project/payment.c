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


INT16U payment_type;
INT8U que_buffer;
INT16U adc_value;
INT16U digi_pulses = 0;
INT16U total_cash = 0;
INT16U cash_invalid;
BOOLEAN pulses_clockwise; // = get_digi_direction

INT8U card_last_number;
INT8U card_last_pin;


BOOLEAN is_card_number_even;
BOOLEAN is_pin_even;
BOOLEAN card_valid;
BOOLEAN is_payment_complete = FALSE;

/*****************************   Functions   *******************************/


//INT16U select_payment_type(INT16U payment){
//    if(payment == CARD)
//        return CARD;
//    if(payment== CASH)
//        return CASH;
//}

BOOLEAN get_card_valid(){
    return card_valid;
}

INT8U last_elemet_queue(QueueHandle_t queue, INT16U queue_size){
    INT8U last_element;
    INT8U reverseQueue[queue_size];
    for(INT8U i = 0; i < queue_size; i++){
        xQueueReceive(queue, &reverseQueue[i], 5);
    }
    last_element = reverseQueue[queue_size - 1];
    //write_int16u(last_element);
    return last_element;
}

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
                      card_last_number = last_elemet_queue(Q_CARD, 8);
                      card_last_pin = last_elemet_queue(Q_PIN, 4);
                      //xQueuePeek(Q_CARD, &que_buffer, 0); //Q-key mangler
                      //write_int16u(que_buffer);
                  }

                  if(card_last_number % 2 == 0){
                      is_card_number_even = TRUE;
                  } else {
                      is_card_number_even = FALSE;
                  }

                  if(card_last_pin % 2 == 0){
                      is_pin_even = TRUE;
                  } else {
                      is_pin_even = FALSE;
                  }

                  if((is_card_number_even && !is_pin_even) || (!is_card_number_even && is_pin_even)){ //Valid combinations are: an even card number with odd PIN, or an odd card number with an even PIN.
                      card_valid = TRUE;
                      is_payment_complete = TRUE;
                  }

                  if(card_valid){
                      write_string("card valid!");
                  }

              break;

              case CASH:

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

