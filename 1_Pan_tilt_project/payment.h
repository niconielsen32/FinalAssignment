/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: payment.h
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

#ifndef _PAYMENT_H
  #define _PAYMENT_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

#define CARD 0
#define CASH 1
#define PIN_ODD 1
#define PIN_EVEN 0
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT16U select_payment_type(INT16U);
BOOLEAN payment_complete();
INT16U get_payment_selection();
INT16U get_total_cash();

void payment_task(void* pvParameters);
/****************************** End Of Module *******************************/
#endif
