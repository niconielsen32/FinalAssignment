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
#include "buttons.h"
#include "UserInterface/write.h"
#include "pumping.h"
#include "fuelselect.h"


/*****************************    Defines    *******************************/



/*****************************   Constants   *******************************/

INT16U  button_state;
INT16U counter_timer = 0;
INT16U counter_timer_event = TE_TIMEOUT;


BOOLEAN display_pumping_lcd;


/*****************************   Functions   *******************************/

void set_counter_timer(INT16U time){
    counter_timer = time;
}

void set_button_state(INT16U button){
    button_state = button;
}

INT16U get_button_state(){
    return button_state;
}


void button_task(void* pvParameters){

    TickType_t last_unblock_buttons;
    last_unblock_buttons = xTaskGetTickCount();

    while(1){


        if(get_fuelselect_complete() && !get_pumping_stopped()){

            if(counter_timer){
                         if(! --counter_timer){
                            counter_timer_event = TE_TIMEOUT;
                         }
                    }

            switch(button_state)
              {
                case idle:

                    if(!(GPIO_PORTF_DATA_R & 0x10) && !get_pumping_stopped()) { //sw1 pressed
                        write_string("nozzle_removed ");
                        gfprintf(COM2, "%c%c     Ready!     ", 0x1B, 0x80);
                        gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
                        button_state = nozzle_removal;
                        set_pumping_stopped(FALSE);
                    }
                    break;

                case nozzle_removal:

                    //turn on display
                    //gfprintf(COM2, "%c%c L    PPL   TotP", 0x1B, 0x80);

                    if(!(GPIO_PORTF_DATA_R & 0x01) && !get_pumping_stopped()) { //sw2 pressed
                        write_string("lever_depressed ");
                        //gfprintf(COM2, "%c%c     Lever      ", 0x1B, 0x80);
                        //gfprintf(COM2, "%c%c    Depressed   ", 0x1B, 0xA8);
                        button_state = lever_depressed;
                    }
                    break;

                case lever_depressed:

                    while(!(GPIO_PORTF_DATA_R & 0x01) && !get_pumping_stopped()){ //sw2 depressed

                    }

                    if((GPIO_PORTF_DATA_R & 0x01) && !get_pumping_stopped()) { //sw2 released
                    //gfprintf(COM2, "%c%c     Lever      ", 0x1B, 0x80);
                    //gfprintf(COM2, "%c%c    released    ", 0x1B, 0xA8);
                    button_state = lever_released;
                    write_string("lever_released ");
                    }

                    break;

                case lever_released:

                        if(!(GPIO_PORTF_DATA_R & 0x01) && !get_pumping_stopped()) { //sw2 pressed
                            write_string("lever_depressed ");
                            button_state = lever_depressed;
                        } else if(!(GPIO_PORTF_DATA_R & 0x10) && !get_pumping_stopped()) { //sw1 pressed
                            counter_timer = TIM_200_MSEC;

                        } else if(! --counter_timer){
                            write_string("nozzle_putback ");
                            gfprintf(COM2, "%c%cHave a nice day!", 0x1B, 0x80);
                            gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);
                            set_pumping_stopped(TRUE);

                        }
                    break;
              }
          }
    }
    vTaskDelayUntil(&last_unblock_buttons, pdMS_TO_TICKS(500));
}
/****************************** End Of Module *******************************/
