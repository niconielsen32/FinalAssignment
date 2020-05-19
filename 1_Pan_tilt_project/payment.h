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


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT8U last_elemet_queue(QueueHandle_t queue, INT16U queue_size);
BOOLEAN get_payment_complete();
void set_payment_complete(BOOLEAN payment_complete);
INT16U get_payment_type();

void payment_task(void* pvParameters);
/****************************** End Of Module *******************************/
#endif
