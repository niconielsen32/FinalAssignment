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

#ifndef SPI_H_
#define SPI_H_

/***************************** Include files *******************************/

#include "emp_type.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

void init_spi(void);
void spi_task(void * pvParameters);
void spi_send(INT16U data);
INT16U spi_peek_send_queue();
INT16U spi_receive();
//void send_byte_task(void * pvParameters);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Test function
******************************************************************************/


/****************************** End Of Module *******************************/
#endif /*SPI_H_*/
