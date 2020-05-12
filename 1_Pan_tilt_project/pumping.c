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
#include "emp_type.h"
#include "glob_def.h"
#include "pumping.h"
#include "FreeRTOS.h"
#include "buttons.h"
#include "flowmeter.h"
#include "fuelselect.h"
#include "payment.h"
#include "UserInterface/write.h"
#include "timer.h"
#include "frt10/frt10/inc/timers.h"

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


static INT16U counter_timer_pumping = 0;
INT16U counter_timer_event_pumping;

/*****************************   Functions   *******************************/

INT16U get_pumping_state(){
    return pumping_state;
}

BOOLEAN pumping_running(){
    return pumping_stopped;
}


void pumping_task(void* pvParameters){

    TickType_t last_unblock_pumping;
    last_unblock_pumping = xTaskGetTickCount();


    while(1){


        if(counter_timer_pumping){
             if(! --counter_timer_pumping){
                counter_timer_event_pumping = TE_TIMEOUT;
             }
        }

//        write_int16u(counter_timer_pumping);
//        write_string(" ");

        type_of_payment = select_payment_type(CARD); //input from keypad
        cur_button_state = get_button_state();
        gas_price_temp = get_gas_price();
        total_cash_temp = get_total_cash();
        out_of_cash_cal = gas_price_temp * 0.15;


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



          if(!pumping_stopped){
            switch(pumping_state)
                  {
                    case no_pumping:
                        //selected_gastype = get_gastype_keypad();
                        select_gas_type(selected_gastype);
                        gas_type = get_gas_price();

                        // red led
                        //write_string("no_pumping ");
                        if(cur_button_state == nozzle_removal){
                            pumping_state = pumping_idle;
                        }
                        break;

                    case pumping_idle:

                        //write_string("pumping_idle ");
                        if(cur_button_state == lever_depressed){
                            pumping_state = pumping_start;
                        }
                            //start diplay

                        break;

                    case pumping_start:

                        //xTimerStart(timer1Sec_handle, (TickType_t) 10);



                        //yellow led
                        //reduced speed 2 sec
                        //write_string("pumping_start ");

                        if(! --counter_timer_pumping){
                            pumping_state = pumping_regular;
                        }

                        if(cur_button_state == lever_released){
                           pumping_state = pumping_stop;
                        }

                        break;

                    case pumping_regular:
                            //green led
                            //Regular speed
                       // write_string("pumping_reg ");

                        if(cur_button_state == lever_released){
                            pumping_state = pumping_stop;
                        }
                        break;

                    case pumping_stop:

                        //yellow led
                        //reduced speed 1 sec
                        //write_string("pumping_stop ");
                        if((type_of_payment == CASH) && ((total_cash_temp - total_amount) <= out_of_cash_cal)){
                                if(total_amount == total_cash_temp){
                                        total_pulses_temp = get_total_pulses();
                                        pumping_stopped = TRUE;
                                        pumping_state = no_pumping;
                                }
                        }

                        counter_timer_pumping = TIM_1_SEC;


                        if(counter_timer_event_pumping == TE_TIMEOUT){
                           total_pulses_temp = get_total_pulses();
                           pumping_state = no_pumping;
                        }

                        break;
                  }
          }
          vTaskDelayUntil(&last_unblock_pumping, pdMS_TO_TICKS(1000));
    }
}

/****************************** End Of Module *******************************/

