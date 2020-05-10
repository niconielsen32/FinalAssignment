/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: data.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Global variables and data to uart.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _DATA_H
  #define _DATA_H

/***************************** Include files *******************************/
#include "emp_type.h"
/*****************************    Defines    *******************************/


/*****************************   Global Variables   *******************************/

FP32 leadfree92 = 8.49;
FP32 leadfree95 = 8.79;
FP32 diesel = 8.12;

FP32 liter_of_gas = 0;
FP32 total_amount = 0;

FP32 total_leadfree92 = 0;
FP32 total_leadfree95 = 0;
FP32 total_diesel = 0;

FP32 sum_of_cash = 0;
FP32 sum_of_card = 0;
FP32 total_operating_time = 0;

BOOLEAN payment_complete = FALSE;
BOOLEAN gastype_selected = FALSE;
BOOLEAN pumping_ended = FALSE;


/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/
#endif
