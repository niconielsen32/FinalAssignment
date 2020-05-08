/*
 * PID.h
 *
 *  Created on: 23. apr. 2020
 *      Author: Jonathan
 */

#ifndef PID_H_
#define PID_H_


#define SAMPLING_TIME_MS 10
#define SAMPLING_TIME_SEK (SAMPLING_TIME_MS * 100)
#define VELOCITY_SCALE 100.0f
#define JOYSTICK_CENTER_VALUE 2048
#define MAXIMUM_VOLTAGE 12.0f

void PID_task(void *pvParameters);
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function : runs the controller, updates PWM signal based on encoder feedback
*****************************************************************************/


/****************************** End Of Module *******************************/

/****************************** End Of Module *******************************/

#endif /* PID_H_ */
