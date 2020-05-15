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
#include "UserInterface/write.h"
/*****************************    Defines    *******************************/
//#define no_pumping        0
//#define pumping_start     1
//#define pumping_regular   2
//#define pumping_stop      3
/*****************************   Constants   *******************************/

INT16U type_of_flow;
INT16U total_pulses = 0;
INT16U pulses_regular = 154;
INT16U pulses_reduced = 26;

/*****************************   Functions   *******************************/
INT64U get_total_pulses(){
   return total_pulses;
}

void flowmeter_task(void* pvParameters){

    TickType_t last_unblock_flowmeter;
    last_unblock_flowmeter = xTaskGetTickCount();


    while(1){

        write_int16u(total_pulses);
        write_string(" ");

        type_of_flow = get_pumping_state();


       switch(type_of_flow){
           case no_pumping:
               total_pulses = 0;
               break;

           case pumping_regular:
               total_pulses += pulses_regular;
               break;

           case pumping_start:
               total_pulses += pulses_reduced;
               break;

           case pumping_stop:
               total_pulses += pulses_reduced;
               break;
       }

        vTaskDelayUntil(&last_unblock_flowmeter, pdMS_TO_TICKS(1000));; // der tælles pulser op for hvert sekund
    }
}
/****************************** End Of Module *******************************/
