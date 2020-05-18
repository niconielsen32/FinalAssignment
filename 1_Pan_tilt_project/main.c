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
//#include "ADC.h"
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
#include "scale.h"
#include "analog.h"



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
  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  init_systick();
  init_gpio();
  init_write();
  //init_ADC();
  init_files();
}


TaskHandle_t write_task_handle = NULL;
//TaskHandle_t adc_task_handle = NULL;
TaskHandle_t ai_task_handle = NULL;
TaskHandle_t UI_task_handle = NULL;
TaskHandle_t button_task_handle = NULL;
TaskHandle_t payment_task_handle = NULL;
TaskHandle_t keypad_task_handle = NULL;
TaskHandle_t flowmeter_task_handle = NULL;
TaskHandle_t pumping_task_handle = NULL;
TaskHandle_t lcd_task_handle = NULL;
TaskHandle_t ui_task_handle = NULL;
TaskHandle_t scale_task_handle = NULL;
TaskHandle_t paytype_task_handle = NULL;


int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/

{
    setupHardware();

    TimerHandle_t timer1 = xTimerCreate("1 second timer", pdMS_TO_TICKS(1000), pdTRUE, 0, timer1_callback);
         if (xTimerStart(timer1, 0)==pdPASS) {
             write_string("timer created");
         } else {
             write_string("timer not created");
         }

    xMutex = xSemaphoreCreateMutex();                                                                                // create the mutex and the queues. make sure the handles are defined globally (in glob_def.h for example)

    Q_KEY = xQueueCreate(128, sizeof(INT16U));
    Q_LCD = xQueueCreate(128, sizeof(INT16U));
    Q_CARD = xQueueCreate(8, sizeof(INT16U));
    Q_PIN = xQueueCreate(4, sizeof(INT16U));

    xTaskCreate(write_task , "write", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &write_task_handle);
    xTaskCreate(UI_task, "UI", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &UI_task_handle);
    xTaskCreate(button_task, "buttons", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &button_task_handle);
    xTaskCreate(payment_task, "payment", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &payment_task_handle);
    xTaskCreate(keypad_task, "keypad", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &keypad_task_handle);
    xTaskCreate(flowmeter_task, "flowmeter", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &flowmeter_task_handle);
    xTaskCreate(pumping_task, "pumping_task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &pumping_task_handle);
    xTaskCreate(lcd_task, "lcd", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &lcd_task_handle);
    xTaskCreate(select_pay_type_task, "select_pay_type", USERTASK_STACK_SIZE, NULL, LOW_PRIO, &paytype_task_handle);

    // Start the scheduler.
    // --------------------
    vTaskStartScheduler();

    // Will only get here, if there was insufficient memory.
    // -----------------------------------------------------
    return 1;
}

/****************************** End Of Module *******************************/
