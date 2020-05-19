/*
 * glob_def.h
 *
 *  Created on: 6. maj 2019
 *      Author: test
 */
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"
#include "emp_type.h"
#include "semphr.h"

#ifndef GLOB_DEF_H_
#define GLOB_DEF_H_


// TIMERS

TimerHandle_t timer_pumping;
TimerHandle_t timer_total_pumping;
TimerHandle_t timer_lever;
QueueHandle_t  Q_KEY, Q_LCD, Q_CARD, Q_CARDnumber, Q_PIN;

// SEMAPHORES
xSemaphoreHandle xMutex;


#define FALSE   0
#define TRUE  !FALSE
#define NULL    ((void *)0)
#define NEGATIVE 0
#define POSITIVE 1
#define LEFT    0
#define RIGHT   1

#define idle              0
#define nozzle_removal    1
#define lever_depressed   2
#define lever_released    3
#define nozzle_putback    4

#define TIM_100_MSEC   20
#define TIM_200_MSEC   40
#define TIM_500_MSEC  100
#define TIM_1_SEC     200
#define TIM_2_SEC     400
#define TIM_5_SEC    1000
#define TIM_15_SEC    3000


#define TE_TIMEOUT      1
// GLOBAL

INT16U ai_value;
INT16U adjusted_value;
INT8U scale;
INT16U offset;

//payment
#define CARD 1
#define CASH 2
#define PIN_ODD 1
#define PIN_EVEN 0

#define COM1  0      // UART
#define COM2  1      // LCD
#define COM3  2      // Keyboard




#endif /* GLOB_DEF_H_ */
