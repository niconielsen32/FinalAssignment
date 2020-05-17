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
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT16U get_button_state();


void button_task(void* pvParameters);

/****************************** End Of Module *******************************/
#endif
