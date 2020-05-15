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

/*****************************   Functions   *******************************/

FP32 get_LF92_price();
FP32 get_LF95_price();
FP32 get_Diesel_price();

void set_LF92_price(FP32 LF92price);
void set_LF95_price(FP32 LF95price);
void set_Diesel_price(FP32 Dieselprice);

FP32 get_total_amount();

/****************************** End Of Module *******************************/
#endif
