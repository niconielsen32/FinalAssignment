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
#include "FreeRTOS.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "systick_frt.h"
#include "gpio.h"
#include "UserInterface/UI.h"
#include "UserInterface/write.h"
#include "buttons.h"
#include "payment.h"
#include "keypad.h"
#include "flowmeter.h"
#include "fuelselect.h"
#include "LCD.h"
#include "pumping.h"
#include "digiswitch.h"




/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

#define NUM_TIMERS 3



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
  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  init_systick();
  init_gpio();
  init_write();
  init_files();
}

TaskHandle_t write_task_handle = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
TaskHandle_t report_task_handle = NULL;
=======
//TaskHandle_t report_task_handle = NULL;
>>>>>>> parent of c638706... From_here!
=======
//TaskHandle_t report_task_handle = NULL;
>>>>>>> parent of c638706... From_here!
TaskHandle_t button_task_handle = NULL;
TaskHandle_t payment_task_handle = NULL;
TaskHandle_t keypad_task_handle = NULL;
TaskHandle_t flowmeter_task_handle = NULL;
TaskHandle_t pumping_task_handle = NULL;
TaskHandle_t lcd_task_handle = NULL;
TaskHandle_t digiswitch_task_handle = NULL;
TaskHandle_t scale_task_handle = NULL;
TaskHandle_t fuelselect_task_handle = NULL;



int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/

{
    setupHardware();

    timer_pumping = xTimerCreate("pumping timer", pdMS_TO_TICKS(1000), pdTRUE, 0, pumping_timer_callback);
    timer_total_pumping = xTimerCreate("total pumping timer", pdMS_TO_TICKS(1000), pdTRUE, 0, total_pumping_time_callback);
    timer_lever = xTimerCreate("lever timer", pdMS_TO_TICKS(1000), pdTRUE, 0, lever_timer_callback);



    xMutex = xSemaphoreCreateMutex();                                                                                // create the mutex and the queues. make sure the handles are defined globally (in glob_def.h for example)


    Q_KEY = xQueueCreate(128, sizeof(INT8U));
    Q_LCD = xQueueCreate(128, sizeof(INT16U));
    Q_CARD = xQueueCreate(8, sizeof(INT8U));
    Q_CARDnumber = xQueueCreate(8, sizeof(INT8U));
    Q_PIN = xQueueCreate(4, sizeof(INT8U));

    xTaskCreate(write_task , "write", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &write_task_handle);
    xTaskCreate(button_task, "buttons", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &button_task_handle);
    xTaskCreate(payment_task, "payment", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &payment_task_handle);
    xTaskCreate(keypad_task, "keypad", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &keypad_task_handle);
    xTaskCreate(flowmeter_task, "flowmeter", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &flowmeter_task_handle);
    xTaskCreate(pumping_task, "pumping_task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &pumping_task_handle);
    xTaskCreate(lcd_task, "lcd", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &lcd_task_handle);
    xTaskCreate(digiswitch_task, "digiswitch", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &digiswitch_task_handle);
    xTaskCreate(fuelselect_task, "select_pay_type", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &fuelselect_task_handle);
    xTaskCreate(report_task, "report", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &report_task_handle);


    // Start the scheduler.
    // --------------------
    vTaskStartScheduler();

    // Will only get here, if there was insufficient memory.
    // -----------------------------------------------------
    return 1;
}

/****************************** End Of Module *******************************/
