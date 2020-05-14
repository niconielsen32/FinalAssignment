/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: tmodel.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Defines the elemtn of the task model..
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 101004  MoH   Module created.
*
*****************************************************************************/

#ifndef _TMODEL_H_
#define _TMODEL_H_


// Tasks.
// ------
#define TASK_RTC         USER_TASK
#define TASK_DISPLAY_RTC USER_TASK+1
#define TASK_LCD         USER_TASK+2
#define TASK_UART_RX     USER_TASK+3
#define TASK_UI          USER_TASK+4
#define TASK_UART_TX     USER_TASK+5
#define TASK_KEY         USER_TASK+6
#define TASK_UI_KEY      USER_TASK+7

typedef unsigned char  HANDLE;      // HANDLE: Task diagram element ID
typedef unsigned char  SEM;        //

#define MAX_TASKS      16
#define MAX_QUEUES     16
#define MAX_SEMAPHORES 64       // The first 2xMAX_QUEUES are reserved for the queues.

#define USER_TASK      1
#define USER_QUEUE     0
#define USER_SEM       2*MAX_QUEUES

#define QUEUE_SIZE   128

#define WAIT_FOREVER  0


// Interrupt Service Routines.
// ---------------------------
#define ISR_TIMER 21

// Semaphores.
// -----------
#define SEM_LCD          USER_SEM
#define SEM_RTC_UPDATED  USER_SEM+1


// Shared State Memory.
// --------------------
#define SSM_RTC_SEC            31
#define SSM_RTC_MIN            32
#define SSM_RTC_HOUR           33

// QUEUEs.
// -------
#define Q_UART_TX   USER_QUEUE
#define Q_UART_RX   USER_QUEUE+1
#define Q_LCD       USER_QUEUE+2
#define Q_KEY       USER_QUEUE+3


#endif /* _TMODEL_H_ */
