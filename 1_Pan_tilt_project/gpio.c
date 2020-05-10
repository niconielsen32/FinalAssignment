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
       SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF; // 1) activate clock for Port F
       dummy = SYSCTL_RCGC2_R;           // allow time for clock to start
       GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
       GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
       GPIO_PORTF_DIR_R = 0x0E;
       GPIO_PORTF_DEN_R = 0x1F;
       GPIO_PORTF_PUR_R = 0x11;

//      GPIO_PORTF_DIR_R  = 0x0a; //PF3, PF1 as output PF4, PF0 as input
//
//      GPIO_PORTF_DEN_R = 0x1b;//PF4, PF3, PF1, PF0 as digital pins
//
//      GPIO_PORTF_PUR_R = 0x11; //Enable pull up for PF0,PF1



//  // Enable the GPIO port that is used for the on-board LED.
//  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF | SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOA;
//
//
//  // Do a dummy read to insert a few cycles after enabling the peripheral.
//  dummy = SYSCTL_RCGC2_R;
//
//  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
//  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
//
//  // Set the direction as output (PF1, PF2 and PF3).
//  GPIO_PORTC_DIR_R |= 0xF0;
//  GPIO_PORTD_DIR_R |= 0x4C;
//  GPIO_PORTF_DIR_R |= 0x0E;
//  //GPIO_PORTA_DIR_R = 0x20;
//
//  // Enable the GPIO pins for digital function (PF0, PF1, PF2, PF3, PF4).
//  GPIO_PORTC_DEN_R |= 0xF0;
//  GPIO_PORTD_DEN_R |= 0x4C;
//
//  GPIO_PORTF_DEN_R |= 0x1F;
//  GPIO_PORTA_DEN_R |= 0x3C;
//
//  // Enable internal pull-up (PF0 and PF4).
//  GPIO_PORTF_PUR_R |= 0x11;
//  //GPIO_PORTA_PUR_R |= 0x3C; // <------------- kan m�ske give fejl.
//
//  // Set port A til alternativ funktion (extern control )
//  GPIO_PORTA_AFSEL_R |= 0x3C;
//
//  //
//  GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA2_SSI0CLK | GPIO_PCTL_PA3_SSI0FSS | GPIO_PCTL_PA4_SSI0RX | GPIO_PCTL_PA5_SSI0TX;
}

/****************************** End Of Module *******************************/
