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
INT16U seconds = 0;
INT16U total_pumping_time = 0;
INT16U total_liters = 0;
INT16U pulses_pr_litr = 512;


/*****************************   Functions   *******************************/

INT16U get_pumping_state(){
    return pumping_state;
}

BOOLEAN pumping_running(){
    return pumping_stopped;
}

void pumping_timer_callback(TimerHandle_t timer) {
    seconds--;
}

void total_pumping_time_callback(TimerHandle_t timer) {
    total_pumping_time++;
}
void pumping_task(void* pvParameters){

    TickType_t last_unblock_pumping;
    last_unblock_pumping = xTaskGetTickCount();


    while(1){

//write_int16u(total_pumping_time);
//write_string(" ");

        type_of_payment = select_payment_type(CARD); //input from keypad
        cur_button_state = get_button_state();
        gas_price_temp = get_gas_price();
        total_cash_temp = get_total_cash();
        out_of_cash_cal = gas_price_temp * 0.15;

        write_int16u(total_pulses_temp);
        if(pumping_stopped){
            total_liters = total_pulses_temp / 512;
            total_amount = total_liters * gas_price_temp;
            write_fp32(total_amount);
        }
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

                        GPIO_PORTF_DATA_R = 0x02; //red
                        seconds = 0;
                        //selected_gastype = get_gastype_keypad();
                        select_gas_type(selected_gastype);
                        gas_type = get_gas_price();

                        // red led
                        //write_string("no ");
                        if(cur_button_state == nozzle_removal){
                            xTimerStart(timer_total_pumping, 0);
                            pumping_state = pumping_idle;
                        }
                        break;

                    case pumping_idle:

                        GPIO_PORTF_DATA_R = 0x02; //red
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
                           pumping_state = pumping_stop;
                        }
                        break;

                    case pumping_regular:

                        GPIO_PORTF_DATA_R = 0x08; //green
                        //write_string("regu ");
                        if(cur_button_state == lever_released){
                            xTimerStart(timer_pumping, 0);
                            seconds = 1;
                            pumping_state = pumping_stop;
                        }
                        break;

                    case pumping_stop:

                        GPIO_PORTF_DATA_R = 0x04; //yellow
                        //write_string("stop ");
                        if(seconds == 0){

                           //////VENT MED AT G� TIL NO PUMPING F�R AT DER ER G�ET 5 SEC, DER SKAL V�RE MULIGHED FOR AT KUNNE TRYKKE LEVER NED IGEN EFTER DEN ER SLUPPET
                           xTimerStop(timer_pumping, 0);
                        }
                        if((type_of_payment == CASH) && ((total_cash_temp - total_amount) <= out_of_cash_cal)){
                                if(total_amount == total_cash_temp){
                                    total_pulses_temp = get_total_pulses();
                                    pumping_stopped = TRUE;
                                }
                        }
                        if(cur_button_state == nozzle_removal){
                            xTimerStop(timer_total_pumping, 0);
                            total_pulses_temp = get_total_pulses();
                            pumping_state = no_pumping;
                            pumping_stopped = TRUE;
                       }
                        break;
                  }
          }
       vTaskDelayUntil(&last_unblock_pumping, pdMS_TO_TICKS(1000));
    }
}

/****************************** End Of Module *******************************/

