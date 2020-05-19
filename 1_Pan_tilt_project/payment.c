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
#include "string.h"
#include "UserInterface/write.h"
#include "LCD.h"
#include "file.h"
#include "keypad.h"
#include "queue.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

INT16U que_buffer;
INT16U stop_payment;
INT16U payment_type;
INT16U adc_value;
INT16U digi_pulses = 0;
INT16U cash_invalid;
BOOLEAN pulses_clockwise; // = get_digi_direction

INT8U card_last_number;
INT8U card_last_pin;

INT16U total_cash_from_digi;

BOOLEAN is_card_number_even;
BOOLEAN is_pin_even;
BOOLEAN card_valid;
BOOLEAN is_payment_complete = FALSE;

/*****************************   Functions   *******************************/


//INT16U select_payment_type(INT16U payment){
//

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

        payment_type = get_pay_type();
        stop_payment = get_payment_stop();

        switch(payment_type){

              case CARD:
                  if(get_paytype_complete()){
                      //write_string("weout");
                      card_last_number = last_elemet_queue(Q_CARD, 8);
                      //write_int16u(card_last_number);
                      card_last_pin = last_elemet_queue(Q_PIN, 4);
                      //write_int16u(card_last_pin);
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
                        write_string(" card valid! ");
                    }

              break;

              case CASH:

                      if(stop_payment){
                         total_cash_from_digi = get_total_cash_from_digi();
                         is_payment_complete = TRUE;
                      }

              break;

        }

    }
}
/****************************** End Of Module *******************************/

