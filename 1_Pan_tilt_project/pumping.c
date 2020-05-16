/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: pumping.c
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
#include "pumping.h"
#include "buttons.h"
#include "flowmeter.h"
#include "fuelselect.h"
#include "payment.h"
#include "UserInterface/write.h"
#include "timer.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/
static INT16U  pumping_state = no_pumping;

FP32 total_amount;
INT16U gas_type;
FP32 gas_price_temp;
INT16U selected_gastype;
INT16U cur_button_state;
INT16U type_of_payment;
FP32 out_of_cash_cal;
FP32 total_cash_temp;
INT16U total_pulses_temp = 0;
FP32 total_liters = 0;
FP32 pulses_pr_liter = 512.0;
FP32 SEC3_reduced = 0.15;

INT16U seconds_lever = 0;
INT16U seconds = 0;
INT16U total_pumping_time = 0;

BOOLEAN pumping_stopped;

/*****************************   Functions   *******************************/


BOOLEAN get_pumping_stopped(){
    return pumping_stopped;
}

void set_pumping_stopped(BOOLEAN pumping_bool){
    pumping_stopped = pumping_bool;
}

INT16U get_pumping_state(){
    return pumping_state;
}

void pumping_timer_callback(TimerHandle_t timer) {
    seconds--;
}

void total_pumping_time_callback(TimerHandle_t timer) {
    total_pumping_time++;
}

void lever_timer_callback(TimerHandle_t timer){
    seconds_lever++;
}

void pumping_task(void* pvParameters){

    TickType_t last_unblock_pumping;
    last_unblock_pumping = xTaskGetTickCount();


    while(1){

//write_int16u(total_pumping_time);
//write_string(" ");

        type_of_payment = get_payment_type(); //input from keypad
        cur_button_state = get_button_state();
        gas_price_temp = get_gas_price();

        total_cash_temp = get_total_cash();
        out_of_cash_cal = gas_price_temp * SEC3_reduced;

        //write_int16u(total_pulses_temp);

//        if(cur_button_state == lever_released){
//            counter_timer_pumping = TIM_15_SEC;
//
//            if(counter_timer_event_pumping == TE_TIMEOUT){
//                pumping_stopped = TRUE;
//            }
//
//            if(type_of_payment == CARD){
//                        //pumping is stopped by releasing the lever for more than 5 seconds.
//                if(counter_timer_pumping >= TIM_5_SEC){
//                    pumping_stopped = TRUE;
//                }
//
//            } else {//pumping is stopped either by releasing the lever or by reaching the prepaid cash
//                    pumping_stopped = TRUE;
//            }
//        }

            if(get_pumping_stopped() == TRUE){
                xTimerStop(timer_total_pumping, 0);
                total_pulses_temp = total_pulses;
                total_liters = total_pulses_temp / pulses_pr_liter;
                total_amount = total_liters * gas_price_temp;
                pumping_state = no_pumping;
            } else {

                switch(pumping_state)
                  {
                    case no_pumping:

                        GPIO_PORTF_DATA_R = 0x02; //red
                        seconds = 0;
                        //selected_gastype = get_gastype_keypad();
                        select_gas_type(selected_gastype);

                        // red led
                        //write_string("no ");
                        if(cur_button_state == nozzle_removal){
                            total_pulses = 0;
                            xTimerStart(timer_total_pumping, 0);
                            pumping_state = pumping_idle;
                        }

                        break;

                    case pumping_idle:

                        if(seconds_lever == 15){
                            seconds_lever = 0;
                            write_string("15sec timeout");
                            set_pumping_stopped(TRUE); //DER SKAL LAVES EN NY TANKNING
                        }

                        if((seconds_lever == 5) && (get_payment_type() == CARD)){
                            seconds_lever = 0;
                            write_string("5sec timeout");
                            set_pumping_stopped(TRUE); //DER SKAL LAVES EN NY TANKNING
                        }

                       // write_string("idle ");
                        if(cur_button_state == lever_depressed){
                            xTimerStart(timer_pumping, 0);
                            seconds = 2;
                            pumping_state = pumping_start;
                        }
                        //lcd = LCD_POWER_UP;
                        break;

                    case pumping_start:

                        GPIO_PORTF_DATA_R = 0x04; //yellow
                        //write_string("start ");
                        if(seconds == 0){
                            xTimerStop(timer_pumping, 0);
                            pumping_state = pumping_regular;
                        }
                        if(cur_button_state == lever_released){
                           xTimerStart(timer_lever, 0);
                           pumping_state = pumping_stop;
                        }
                        break;

                    case pumping_regular:

                        GPIO_PORTF_DATA_R = 0x08; //green
                        //write_string("regu ");
                        if(cur_button_state == lever_released){
                            xTimerStart(timer_lever, 0);
                            xTimerStart(timer_pumping, 0);
                            seconds = 1;
                            pumping_state = pumping_stop;
                        }

                        if((type_of_payment == CASH) && ((total_cash_temp - total_amount) <= out_of_cash_cal)){
                           if(total_amount == total_cash_temp){
                               total_pulses_temp = get_total_pulses();
                               set_pumping_stopped(TRUE);
                           }
                        }

                        break;

                    case pumping_stop:

                        GPIO_PORTF_DATA_R = 0x04; //yellow
                        //write_string("stop ");

                        if(seconds == 0){
                           xTimerStop(timer_pumping, 0);
                           pumping_state = pumping_idle;
                        }

                        break;
                  }
          }
       vTaskDelayUntil(&last_unblock_pumping, pdMS_TO_TICKS(100));
    }
}

/****************************** End Of Module *******************************/

