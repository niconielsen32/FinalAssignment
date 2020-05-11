/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: main.c
*
* PROJECT....: EMP
*
* DESCRIPTION: Assignment 2, main module. No main.h file.
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
#include <UserInterface/leds.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick_frt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ADC.h"
#include "gpio.h"
#include "UserInterface/UI.h"
#include "UserInterface/write.h"
#include "buttons.h"





/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{
  // TODO: Put hardware configuration and initialisation in here

  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  init_systick();
  init_gpio();
  init_write();
  init_ADC();
  //init_files(); // for printf....
}


TaskHandle_t write_task_handle = NULL;
TaskHandle_t adc_task_handle = NULL;
TaskHandle_t ui_task_handle = NULL;
TaskHandle_t button_task_handle = NULL;


int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{

    setupHardware();


    xTaskCreate(write_task , "write", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &write_task_handle);
//    xTaskCreate( status_led_task, "Red_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
    xTaskCreate(adc_read_task,  "ADC_read",  USERTASK_STACK_SIZE, NULL, LOW_PRIO, &adc_task_handle);
    xTaskCreate(UI_task, "UI", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &ui_task_handle);
    xTaskCreate(button_task, "buttons", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &button_task_handle);


    // Start the scheduler.
    // --------------------
    vTaskStartScheduler();

    // Will only get here, if there was insufficient memory.
    // -----------------------------------------------------
    return 1;
}

/****************************** End Of Module *******************************/
