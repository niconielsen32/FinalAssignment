/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: flowmeter.c
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
#include "flowmeter.h"
#include "pumping.h"
/*****************************    Defines    *******************************/
//#define no_pumping        0
//#define pumping_start     1
//#define pumping_regular   2
//#define pumping_stop      3
/*****************************   Constants   *******************************/

INT16U type_of_flow;
INT64U total_pulses = 0;

/*****************************   Functions   *******************************/
INT64U get_total_pulses(){
   return total_pulses;
}

void flowmeter_task(void* pvParameters){

    while(1){
        type_of_flow = get_pumping_state();
        //1 løsning
        if(type_of_flow == pumping_regular){
            total_pulses += 154;
        }
        if(type_of_flow == (pumping_start | pumping_stop)){
            total_pulses += 26;
        }

//        //alternativ løsning
//        total_pulses += 512;
//        // udregningerne og resten laves under pumping state machinen.
    }
    vTaskDelay(pdMS_TO_TICKS(1000)); // der tælles pulser op for hvert sekund
}
/****************************** End Of Module *******************************/
