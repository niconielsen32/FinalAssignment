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

//  QUEUES
QueueHandle_t  Q_KEY, Q_LCD;

// SEMAPHORES
xSemaphoreHandle xMutex;

#define FALSE   0
#define TRUE  !FALSE
#define NULL    ((void *)0)
#define NEGATIVE 0
#define POSITIVE 1
#define LEFT    0
#define RIGHT   1

// GLOBAL

INT16U ai_value;
INT16U adjusted_value;
INT8U scale;
INT16U offset;



#endif /* GLOB_DEF_H_ */
