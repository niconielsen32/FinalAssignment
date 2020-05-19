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
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "FreeRTOS.h"
#include "buttons.h"
#include "UserInterface/write.h"
#include "pumping.h"
#include "LCD.h"
#include <stdint.h>


/*****************************    Defines    *******************************/



/*****************************   Constants   *******************************/

static INT16U  button_state = idle;
static INT16U counter_timer = 0;
INT16U counter_timer_event = TE_TIMEOUT;
<<<<<<< HEAD
BOOLEAN payment_stop = FALSE;
=======
INT16U running_pulses;
>>>>>>> fuelselect_task


/*****************************   Functions   *******************************/


INT16U get_button_state(){
    return button_state;
}

INT16U get_payment_stop(){
    return payment_stop;
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

        //GPIO_PORTF_DATA_R &= 0xF7;
        switch(button_state)
          {
            case idle:

                if(!(GPIO_PORTF_DATA_R & 0x10)) { //sw1 pressed
<<<<<<< HEAD
=======
                    //GPIO_PORTF_DATA_R = 0x04; //put it on the YELLOW LED
                    button_state = nozzle_removal;
                    pumping_stopped = FALSE;
                    //write_int16u(get_pay_type());
>>>>>>> fuelselect_task
                    //write_string("nozzle_removed ");
                    button_state = nozzle_removal;
                    set_pumping_stopped(FALSE);
                    payment_stop = TRUE;
                }
                break;

            case nozzle_removal:
<<<<<<< HEAD
=======
                //turn on display
                running_pulses = get_total_pulses(); // = 0 here
>>>>>>> fuelselect_task

                if(!(GPIO_PORTF_DATA_R & 0x01)) { //sw2 pressed
                    write_string("lever_depressed ");
                    button_state = lever_depressed;
                }
                break;

            case lever_depressed:

                while(!(GPIO_PORTF_DATA_R & 0x01)){ //sw2 depressed

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
   // vTaskDelayUntil(&last_unblock_buttons, pdMS_TO_TICKS(1SEC));
}
/****************************** End Of Module *******************************/
