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
#define no_pumping              0
#define pumping_idle            1
#define pumping_reduced_2sec    2
#define pumping_regular         3
#define pumping_reduced_1sec    4
#define pumping_stop            5

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
INT16U get_pumping_state();

BOOLEAN get_pumping_stopped();
void set_pumping_stopped(BOOLEAN pumping_bool);

void pumping_timer_callback(TimerHandle_t timer);
void total_pumping_time_callback(TimerHandle_t timer);
void lever_timer_callback(TimerHandle_t timer);
void set_reduced_last(BOOLEAN reduced);
INT8U get_seconds_lever();
FP32 get_total_liters();
FP32 get_total_amount();
INT16U get_total_cash_temp();

void display_pumping();
void ftoa(float n, char *res, int afterpoint);


void pumping_task(void* pvParameters);
/****************************** End Of Module *******************************/
#endif
