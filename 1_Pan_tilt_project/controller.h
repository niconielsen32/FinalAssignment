/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: controller.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 100408  KHA    Module created.
*
*****************************************************************************/

#ifndef _CONTROLLER_H
  #define _CONTROLLER_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

void controller_task(void *pvParameters);
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function : runs the controller, updates PWM signal based on encoder feedback
*****************************************************************************/


/****************************** End Of Module *******************************/
#endif

