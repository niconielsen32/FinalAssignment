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

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/
static INT16U  pumping_state = no_pumping;

BOOLEAN pumping_interrupted = FALSE;
/*****************************   Functions   *******************************/

INT16U get_pumping_state(){
    return pumping_state;
}

BOOLEAN pumping_running(){
    return pumping_interrupted;
}

void pumping_task(void* pvParameters){

    while(1){

            switch(pumping_state)
                  {
                    case no_pumping:

                        break;

                    case pumping_start:

                        break;

                    case pumping_regular:

                        break;

                    case pumping_stop:

                        break;
                  }

    }
}

/****************************** End Of Module *******************************/

