/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: payment.c
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
#include "payment.h"

/*****************************    Defines    *******************************/
#define CARD 0
#define CASH 1

/*****************************   Constants   *******************************/
INT16U payment_type;

BOOLEAN is_payment_complete = FALSE;
/*****************************   Functions   *******************************/
INT16U get_payment_type(){
    return payment_type;
}
BOOLEAN payment_complete(){
    return is_payment_complete;
}

void payment_task(void* pvParameters){

    while(1){


    }
}
/****************************** End Of Module *******************************/

