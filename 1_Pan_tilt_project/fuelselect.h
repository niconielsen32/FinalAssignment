/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: gasstation.h
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

#ifndef _FUELSELECT_H
  #define _FUELSELECT_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

void select_gas_type(INT16U);
INT16U get_gas_type();
FP32 get_gas_price();
void set_gas_price(INT16U gastype, FP32 gasprice);

BOOLEAN get_fuelselect_complete();
void fuelselect_task(void* pvParameters);

/****************************** End Of Module *******************************/
#endif
