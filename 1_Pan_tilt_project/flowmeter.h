/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: flowmeter.h
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

#ifndef _FLOWMETER_H
  #define _FLOWMETER_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

INT16U total_pulses;
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

INT64U get_total_pulses();

void flowmeter_task(void* pvParameters);

/****************************** End Of Module *******************************/
#endif
