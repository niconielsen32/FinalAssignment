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
#define pumping_idle      1
#define pumping_start     2
#define pumping_regular   3
#define pumping_stop      4

/*****************************   Constants   *******************************/
BOOLEAN pumping_stopped;

/*****************************   Functions   *******************************/
INT16U get_pumping_state();
BOOLEAN pumping_running();

static void vTimerCallback1SecExpired( TimerHandle_t xTimer );

void pumping_task(void* pvParameters);
/****************************** End Of Module *******************************/
#endif
