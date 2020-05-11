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
#include <stdint.h>


/*****************************    Defines    *******************************/
#define idle              0
#define nozzle_removal    1
#define lever_depressed   2
#define lever_released    3

#define TIM_100_MSEC   20
#define TIM_200_MSEC   40
#define TIM_500_MSEC  100
#define TIM_1_SEC     200
#define TIM_2_SEC     400

#define TE_TIMEOUT      1


/*****************************   Constants   *******************************/

static INT16U  button_state = idle;
static INT16U counter_timer = 0;
INT16U counter_timer_event = TE_TIMEOUT;

BOOLEAN pumping = FALSE;

INT8U karakter;

INT8U event;


/*****************************   Functions   *******************************/

INT16U get_button_state(){
    return button_state;
}
void button_task(void* pvParameters){


    while(1){

        if(counter_timer){
             if(! --counter_timer){
                counter_timer_event = TE_TIMEOUT;
             }
        }
        GPIO_PORTF_DATA_R &= 0xF7;
        switch(button_state)
          {
            case idle:
                if(!(GPIO_PORTF_DATA_R & 0x10)) { //sw1 pressed
                    GPIO_PORTF_DATA_R = 0x04; //put it on the YELLOW LED
                    button_state = nozzle_removal;
                    write_string("nozzle_removed ");
                }
                break;
            case nozzle_removal:
                if(!(GPIO_PORTF_DATA_R & 0x01)) { //sw2 pressed
                    GPIO_PORTF_DATA_R = 0x02; //put it on the GREEN LED
                    button_state = lever_depressed;
                    write_string("lever_depressed ");
                }
                break;
            case lever_depressed:
                while(!(GPIO_PORTF_DATA_R & 0x01)){ //sw2 depressed
                    pumping = TRUE;
                    GPIO_PORTF_DATA_R = 0x08; //put it on the RED LED
                }
                if((GPIO_PORTF_DATA_R & 0x01)) { //sw2 released
                    GPIO_PORTF_DATA_R = 0x02; //put it on the GREEN LED
                    button_state = lever_released;
                    write_string("lever_released ");
                }
                break;
            case lever_released:
                if(!(GPIO_PORTF_DATA_R & 0x10)) { //sw1 pressed
                    GPIO_PORTF_DATA_R = 0x04; //put it on the YELLOW LED
                    counter_timer = TIM_100_MSEC;
                } else if(! --counter_timer){
                    write_string("idle ");
                    button_state = idle;
                }
                break;
          }
        }
    vTaskDelay(pdMS_TO_TICKS(500));
}
/****************************** End Of Module *******************************/
