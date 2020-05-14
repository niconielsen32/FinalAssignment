/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.:timer.h
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

#ifndef _TIMER_H
  #define _TIMER_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

#define TIM_100_MSEC   20
#define TIM_200_MSEC   40
#define TIM_500_MSEC  100
#define TIM_1_SEC     200
#define TIM_2_SEC     400

#define TE_TIMEOUT      1
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

BOOLEAN time_out(INT16U counter_value);

/****************************** End Of Module *******************************/
#endif
