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
#include "buttons.h"
#include "flowmeter.h"
#include "fuelselect.h"
#include "payment.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/
static INT16U  pumping_state = no_pumping;

BOOLEAN pumping_interrupted = FALSE;

INT16U gas_type;
INT16U selected_gastype;
INT16U cur_button_state;
/*****************************   Functions   *******************************/

INT16U get_pumping_state(){
    return pumping_state;
}

BOOLEAN pumping_running(){
    return pumping_interrupted;
}

void pumping_task(void* pvParameters){

    while(1){
        cur_button_state = get_button_state();
            switch(pumping_state)
                  {
                    case no_pumping:
                        //selected_gastype = get_gastype_keypad();
                        select_gas_type(selected_gastype);
                        gas_type = get_gas_price();

                        if(cur_button_state == nozzle_removal)
                            pumping_state = pumping_idle;

                        break;

                    case pumping_idle:
                        if(cur_button_state == lever_depressed)
                            pumping_state = pumping_start;
                            //start diplay

                        break;

                    case pumping_start:
                        //if(timer < 2 sec)
                            //yellow led
                            //reduced speed 2 sec
                        //else
                            pumping_state = pumping_regular;
                            if(cur_button_state == lever_released){
                               pumping_state = pumping_stop;
                            }
                        break;

                    case pumping_regular:
                        //green led
                        //Regular speed
                        if(cur_button_state == lever_released){
                            pumping_state = pumping_stop;
                        }
                        break;

                    case pumping_stop:
                        //if(timer < 1 sec)
                            //yellow led
                            //reduced speed 1 sec
                        //else
                        pumping_state = no_pumping;

                        break;
                  }

    }
}

/****************************** End Of Module *******************************/

