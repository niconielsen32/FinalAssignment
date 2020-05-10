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
#include "buttons.h"
#include <stdint.h>


/*****************************    Defines    *******************************/
#define idle              0
#define nozzle_removal    1
#define lever_depressed   2
#define lever_released    3


/*****************************   Constants   *******************************/

static INT16U  button_state = idle;

BOOLEAN pumping = FALSE;

INT8U karakter;

/*****************************   Functions   *******************************/

INT8U get_button_state(){
    return button_state;
}
void button_task(void * pvParameters){

    while(1){

        switch(button_state)
          {
            case idle:
                if(!(GPIO_PORTF_DATA_R & 0x10)) { //sw1 pressed
                    GPIO_PORTF_DATA_R = 0x02; //put it on the RED LED
                    button_state = nozzle_removal;
                }
            case nozzle_removal:
                if(!(GPIO_PORTF_DATA_R & 0x01)) { //sw2 pressed
                    GPIO_PORTF_DATA_R = 0x06; //put it on the RED LED
                    button_state = lever_depressed;
                }
            case lever_depressed:
                while(!(GPIO_PORTF_DATA_R & 0x01)){ //sw2 depressed
                    pumping = FALSE;
                    GPIO_PORTF_DATA_R = 0x08; //put it on the RED LED
                }
                if(GPIO_PORTF_DATA_R & 0x01){ //sw2 pressed
                    button_state = lever_released;
                }
            case lever_released:
                if(!(GPIO_PORTF_DATA_R & 0x10)) { //sw1 pressed
                    GPIO_PORTF_DATA_R = 0x00;
                    button_state = idle;
                }

                break;

          }
        }
}
/****************************** End Of Module *******************************/
