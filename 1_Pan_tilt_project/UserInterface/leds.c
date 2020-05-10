/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: leds.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "semphr.h"
#include "emp_type.h"
#include "string.h"
#include "ADC.h"
#include "leds.h"



/*****************************    Defines    *******************************/

#define RED 0x02

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void red_led_init(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
    INT8S dummy;
    // Enable the GPIO port that is used for the on-board LED.
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

    // Do a dummy read to insert a few cycles after enabling the peripheral.
    dummy = SYSCTL_RCGC2_R;

    GPIO_PORTF_DIR_R |= 0x02;
    GPIO_PORTF_DEN_R |= 0x02;
}



void status_led_task(void *pvParameters)
{
    red_led_init();
    INT16U value = pdMS_TO_TICKS( 10 );
//    gfprintf(COM1, "Ticks,   A  \n \r"); //Angiver hvad der printes i kolonnerne
    while(1)
    {
    // Toggle red led
    GPIO_PORTF_DATA_R ^= RED;


//    gfprintf( COM1, "%2d,     %2d, \n \r", pwm_motor1, pwm_motor2); //Printer ticks og værdien A = adc_get_x() (sat til 5) i en kolonne med mellemrum 2

    value += 10;

    vTaskDelay( pdMS_TO_TICKS( 2000 ));
  }
}



/****************************** End Of Module *******************************/




