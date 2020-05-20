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
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "payment.h"
#include "buttons.h"
#include "LCD.h"
#include "gpio.h"
#include "string.h"
#include "digiswitch.h"
#include "string.h"
#include "UserInterface/write.h"
#include "UserInterface/UI.h"
#include "LCD.h"
#include "file.h"
#include "keypad.h"
#include "queue.h"
#include "fuelselect.h"
#include "pumping.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

INT16U cash_invalid;

INT8U card_last_number;
INT8U card_last_pin;

INT16U total_cash_from_digi;

BOOLEAN is_card_number_even;
BOOLEAN is_pin_even;
BOOLEAN card_valid;
BOOLEAN is_payment_complete;

INT16U payment_type;

BOOLEAN paytype_complete;

INT8U pay_state = 0;
INT8U order = 0;
INT8U card_cif;
INT8U pin_cif;
INT8U card_try;        //<-- If pin is wrong 3 times

INT16U type;


/*****************************   Functions   *******************************/

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

BOOLEAN get_paytype_complete(){
    return paytype_complete;
}


void terminate_session(){
    UI_receipt();
    xQueueReset(Q_CARD);
    xQueueReset(Q_PIN);
    pay_state = 0;
    order = 0;
    paytype_complete = FALSE;
    set_payment_complete(FALSE);
    set_fuelselect_complete(FALSE);
    set_pumping_stopped(FALSE);
    set_digi_complete(FALSE);
    set_total_cash_from_digi(0);
    set_reduced_last(FALSE);
    set_button_state(idle);
}

void payment_task(void* pvParameters){

    TickType_t last_unblock_payment;
    last_unblock_payment = xTaskGetTickCount();

    while(1){

        if(!paytype_complete){

            INT8U key = 0;

               switch(pay_state)
               {

               case 0:
                   gfprintf(COM2, "%c%cCard: Press one ", 0x1B, 0x80);
                   gfprintf(COM2, "%c%cCash: Press two ", 0x1B, 0xA8);          // "Scale:" is printed on the second line of the display
                   key = get_keyboard();                                       // we get a value from the keyboard
                   if( key >= '1' && key <= '2')                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
                   {
                       type = key - '0';
                       // the value from the keyboard is given as an ASCII char, so to convert to the actual value we subtract the ASCII-value for 0
                       pay_state = 1;

                   }
                   break;

               case 1:
                   gfprintf(COM2, "%c%cYou have method: ", 0x1B, 0x80);                  // "Offset:" is printed on the second line of the display                                      // same procedure as in state 0, but we save the value in off1 since we want it as the first digit of the offset value

                   if( type == CARD)
                   {
                       write_string("CARD");
                       gfprintf(COM2, "%c%c     Card      ", 0x1B, 0xA8);              // the digit is printed on the second line (after "Offset:")
                       payment_type = CARD;
                       pay_state = 2;

                   }else if ( type == CASH){
                       write_string("CASH");// again we subtract the ASCII for 0. we also multiply by 100 since it's the first of the 3 digits
                       gfprintf(COM2, "%c%c     Cash      ", 0x1B, 0xA8);
                       payment_type = CASH;

                       gfprintf(COM2, "%c%c   Total Cash    ", 0x1B, 0x80);
                       gfprintf(COM2, "%c%c       0         ", 0x1B, 0xA8);
                       pay_state = 3;
                       break;

                   } else{
                       gfprintf(COM2, "%c%c", 0x1B, 0xA8);

                   }
                   break;

               case 2:

                    switch(order)
                    {

                    case 0:
                        gfprintf(COM2, "%c%cCard number:    ", 0x1B, 0x80);
                        gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);

                        write_string(" card no: ");
                        for(INT8U i = 0; i < 8; i++){

                            key = get_keyboard();
                            if( key >= '0' && key <= '9')                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
                              {
                                gfprintf(COM2, "%c%c%c", 0x1B, 0xC4+i, key);
                                card_cif = key -'0';
                                write_int16u(card_cif);
                                xQueueSend(Q_CARD, &card_cif, 5);
                                xQueueSend(Q_CARDnumber, &card_cif, 5);

                                if (i == 7){
                                    write_string(" card entered ");
                                    order = 1;
                                }
                              } else {
                                  i--;
                              }
                        }
                        break;

                    case 1:

                        gfprintf(COM2, "%c%cPin code:    ", 0x1B, 0x80);
                        gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);

                        write_string("pin: ");

                        for(INT8U j = 0; j < 4; j++){

                            key = get_keyboard();
                            if( key >= '0' && key <= '9')                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
                              {
                                gfprintf(COM2, "%c%c%c", 0x1B, 0xC6+j, key);
                                pin_cif = key - '0';
                                write_int16u(pin_cif);
                                xQueueSend(Q_PIN, &pin_cif, 0);
                                if (j == 3){
                                    write_string(" pin entered ");
                                    pay_state = 3;
                                }
                              } else {
                                  j--;
                              }
                            }

                        break;

                    }
                   break;

               case 3:
                   paytype_complete = TRUE;
                   write_string(" Complete ");

                   break;
               }
             }

        if(paytype_complete){
            switch(payment_type){

                  case CARD:

                      card_last_number = last_elemet_queue(Q_CARD, 8);
                      card_last_pin = last_elemet_queue(Q_PIN, 4);

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

                        if(!card_valid){
                            write_string("card invalid!");
                            gfprintf(COM2, "%c%cCard not Valid! ", 0x1B, 0x80);
                            gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);

                            set_pumping_stopped(TRUE);
                        }

                  break;

                  case CASH:

                          if(!(GPIO_PORTA_DATA_R & 0x80)){ //button on digiswitch
                            set_digi_complete(TRUE);
                            is_payment_complete = TRUE;
                          }






                  break;

            }
        }
        vTaskDelayUntil(&last_unblock_payment, pdMS_TO_TICKS(100));
    }
}
/****************************** End Of Module *******************************/

