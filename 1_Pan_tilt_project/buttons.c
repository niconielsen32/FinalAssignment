/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: buttons.c
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
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "FreeRTOS.h"
#include "buttons.h"
#include "UserInterface/write.h"
#include "pumping.h"
#include "LCD.h"
#include "payment.h"
#include "fuelselect.h"
#include "digiswitch.h"
#include "flowmeter.h"


/*****************************    Defines    *******************************/



/*****************************   Constants   *******************************/

static INT16U  button_state = idle;
static INT16U counter_timer = 0;
INT16U counter_timer_event = TE_TIMEOUT;

INT16U running_pulses;
FP32 price_one_liter;
FP32 running_liters;
FP32 running_total_price;
FP32 digi_cash;

BOOLEAN display_pumping_lcd;


/*****************************   Functions   *******************************/
FP32 get_running_liters(){
    return running_liters;
}

FP32 get_price_one_liter(){
    return price_one_liter;
}

FP32 get_running_total_price(){
    return running_total_price;
}

BOOLEAN get_display_pumping_lcd(){
    return display_pumping_lcd;
}

void set_display_pumping_lcd(BOOLEAN display){
    display_pumping_lcd = display;
}


INT16U get_button_state(){
    return button_state;
}


void button_task(void* pvParameters){

    TickType_t last_unblock_buttons;
    last_unblock_buttons = xTaskGetTickCount();

    while(1){

        if(counter_timer){
             if(! --counter_timer){
                counter_timer_event = TE_TIMEOUT;
             }
        }

        if(get_fuelselect_complete()){
            switch(button_state)
              {
                case idle:

                    if(!(GPIO_PORTF_DATA_R & 0x10)) { //sw1 pressed
                        write_string("nozzle_removed ");
                        button_state = nozzle_removal;
                        set_pumping_stopped(FALSE);
                    }
                    break;

                case nozzle_removal:

                    //turn on display
                    running_pulses = 0;
                    price_one_liter = get_gas_price();
                    digi_cash = get_total_cash_from_digi();
                    running_total_price = 0;


                    if(!(GPIO_PORTF_DATA_R & 0x01)) { //sw2 pressed
                        write_string("lever_depressed ");
                        button_state = lever_depressed;
                    }
                    break;

                case lever_depressed:

                    while(!(GPIO_PORTF_DATA_R & 0x01)){ //sw2 depressed

                        running_pulses = get_total_pulses();
                        write_int16u(running_pulses);
                        running_liters = running_pulses / 512.0; // skal skrives ud på LCD et andet sted når display_pumping_lcd == true

                        price_one_liter = get_gas_price(); // skal skrives ud på LCD et andet sted når display_pumping_lcd == true

                        if(get_payment_type() == CARD){
                            running_total_price = running_liters * price_one_liter; // skal skrives ud på LCD et andet sted når display_pumping_lcd == true
                        } else if(get_payment_type() == CASH){
                            running_total_price = digi_cash - (running_liters * price_one_liter);
                        }
//
//                        write_string(" Liters: ");
//                        write_fp32(running_liters);
//
//                        write_string(" Price: ");
//                        write_fp32(price_one_liter);
//
//                        write_string(" Total: ");
//                        write_fp32(running_total_price);


                    }

                    if((GPIO_PORTF_DATA_R & 0x01)) { //sw2 released
                        button_state = lever_released;
                        write_string("lever_released ");
                    }
                    break;

                case lever_released:

                        if(!(GPIO_PORTF_DATA_R & 0x01)) { //sw2 pressed
                            write_string("lever_depressed ");
                            button_state = lever_depressed;
                        } else if(!(GPIO_PORTF_DATA_R & 0x10)) { //sw1 pressed
                            counter_timer = TIM_200_MSEC;
                        } else if(! --counter_timer){
                            write_string("nozzle_putback ");
                            button_state = idle; //GÅ TIL EN NY TANKNING ISTEDET
                            set_pumping_stopped(TRUE);
                        }
                    break;
              }
          }
    }
    vTaskDelayUntil(&last_unblock_buttons, pdMS_TO_TICKS(500));
}
/****************************** End Of Module *******************************/
