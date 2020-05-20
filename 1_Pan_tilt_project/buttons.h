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
void set_counter_timer(INT16U time);

FP32 get_running_pulses();
FP32 get_price_one_liter();
FP32 get_running_total_price();
void set_button_state(INT16U button);

//void set_display_pumping_lcd(BOOLEAN display);
//BOOLEAN get_display_pumping_lcd();

void button_task(void* pvParameters);

/****************************** End Of Module *******************************/
#endif
