/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: buttons.h
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

#ifndef _BUTTONS_H
  #define _BUTTONS_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/
#define idle              0
#define nozzle_removal    1
#define lever_depressed   2
#define lever_released    3
#define nozzle_putback    4

#define TIM_100_MSEC   20
#define TIM_200_MSEC   40
#define TIM_500_MSEC  100
#define TIM_1_SEC     200
#define TIM_2_SEC     400
#define TIM_5_SEC    1000
#define TIM_15_SEC    3000


#define TE_TIMEOUT      1
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT16U get_button_state();


void button_task(void* pvParameters);

/****************************** End Of Module *******************************/
#endif
