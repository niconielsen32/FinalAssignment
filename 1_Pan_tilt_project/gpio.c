/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: gpio.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_gpio(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{
    int dummy;
      // Enable the GPIO ports that is used on the board
      SYSCTL_RCGC2_R  =  SYSCTL_RCGC2_GPIOA |SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOE |SYSCTL_RCGC2_GPIOF;
      SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;

      // Do a dummy read to insert a few cycles after enabling the peripheral.
      dummy = SYSCTL_RCGC2_R;

      //  SW2 settings
      GPIO_PORTF_LOCK_R = 0x4C4F434B;
      GPIO_PORTF_CR_R = 0xFF;

      // Set direction
      GPIO_PORTA_DIR_R = 0x1C; // Set outputs PA2(KEYB D), PA3(KEYB E) and PA4(KEYB F) - Set inputs PA5(DIGI A), PA6(DIGI B) and PA7(DIGI P2)
      GPIO_PORTC_DIR_R = 0xF0; // Set outputs PC4(LCD D4), PC5(LCD D5), PC6(LCD D6) and PC7(LCD D7)
      GPIO_PORTD_DIR_R = 0x4C; // Set outputs PD2(LCD RS), PD3(LCD E) and PD6(Status LED)
      GPIO_PORTF_DIR_R = 0x0E; // Set outputs PF1(Red LED), PF2(Yellow LED) and PF3(Green LED) - Set inputs PF0(sw2) and PF4(sw1)

      //Digital function
      GPIO_PORTA_DEN_R = 0xFC; // Enable digital function (PA2, PA3, PA4, PA5, PA6, PA7)
      GPIO_PORTC_DEN_R = 0xF0; // Enable digital function (PC4, PC5, PC6, PC7).
      GPIO_PORTD_DEN_R = 0x4C; // Enable digital function (PD2, PD3, PD6).
      GPIO_PORTE_DEN_R = 0x0F; // Enable digital function (PE0, PE1, PE2, PE3).
      GPIO_PORTF_DEN_R = 0x1F; // Enable digital function (PF0, PF1, PF2, PF3, PF4).

      // Enable internal pull-up PF0(sw2) and PF4(sw1).
      GPIO_PORTF_PUR_R = 0x11;
}

/****************************** End Of Module *******************************/
