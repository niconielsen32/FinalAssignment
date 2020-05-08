/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: gpio.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

#ifndef ADC_H_
#define ADC_H_

/***************************** Include files *******************************/

#include "emp_type.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

void init_ADC(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/
void adc_read_task(void * pvParameters);
void adc_to_uart(INT16U adc_datax, INT16U adc_datay);
INT16U adc_get_x();
INT16U adc_get_y();

/****************************** End Of Module *******************************/
#endif /*ADC_H_*/
