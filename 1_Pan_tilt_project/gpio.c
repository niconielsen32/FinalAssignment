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
    // Enable the GPIO port that is used for the on-board LED.
      SYSCTL_RCGC2_R  =  SYSCTL_RCGC2_GPIOA |SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOE |SYSCTL_RCGC2_GPIOF;
      SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;

      // Do a dummy read to insert a few cycles after enabling the peripheral.
      dummy = SYSCTL_RCGC2_R;


      //  SW2
      GPIO_PORTF_LOCK_R = 0x4C4F434B;
      GPIO_PORTF_CR_R = 0xFF;

      // Set the direction as output (PF1, PF2 and PF3).
      GPIO_PORTA_DIR_R = 0x1C;
      GPIO_PORTC_DIR_R = 0xF0;
      GPIO_PORTD_DIR_R = 0x4C;
      GPIO_PORTF_DIR_R = 0x0E;

      // Enable the GPIO pins for digital function (PF0, PF1, PF2, PF3, PF4).
      GPIO_PORTA_DEN_R = 0xFC;
      GPIO_PORTC_DEN_R = 0xF0;
      GPIO_PORTD_DEN_R = 0x4C;
      GPIO_PORTE_DEN_R = 0x0F;
      GPIO_PORTF_DEN_R = 0x1F;

      // Enable internal pull-up (PF0 and PF4).
      GPIO_PORTF_PUR_R = 0x11;


////////////////////////////////////////////////////

//    int dummy;
//       SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF; // 1) activate clock for Port F
//       dummy = SYSCTL_RCGC2_R;           // allow time for clock to start
//       GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
//       GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
//       GPIO_PORTF_DIR_R = 0x0E;
//       GPIO_PORTF_DEN_R = 0x1F;
//       GPIO_PORTF_PUR_R = 0x11;
//
//       // Enable the GPIO port that is used for the on-board LED.
//       SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOF;
//
//       // Do a dummy read to insert a few cycles after enabling the peripheral.
//       dummy = SYSCTL_RCGC2_R;
//
//       // Set the direction as output (PF1, PF2 and PF3).
//       GPIO_PORTF_DIR_R = 0x0E;
//       // Set the direction as output (PD6).
//       GPIO_PORTD_DIR_R = 0x40;
//
//       // Enable the GPIO pins for digital function (PF0, PF1, PF2, PF3, PF4).
//       GPIO_PORTF_DEN_R = 0x1F;
//       // Enable the GPIO pins for digital function (PD6).
//       GPIO_PORTD_DEN_R = 0x40;
//
//       // Enable internal pull-up (PF0 and PF4).
//       GPIO_PORTF_PUR_R = 0x11;

//      GPIO_PORTF_DIR_R  = 0x0a; //PF3, PF1 as output PF4, PF0 as input
//
//      GPIO_PORTF_DEN_R = 0x1b;//PF4, PF3, PF1, PF0 as digital pins
//
//      GPIO_PORTF_PUR_R = 0x11; //Enable pull up for PF0,PF1


}

/****************************** End Of Module *******************************/
