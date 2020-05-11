/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: pumping.h
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

#ifndef _PUMPING_H
  #define _PUMPING_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/
#define no_pumping        0
#define pumping_start     1
#define pumping_regular   2
#define pumping_stop      3

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT16U get_pumping_state();
BOOLEAN pumping_running();

void pumping_task(void* pvParameters);
/****************************** End Of Module *******************************/
#endif
