/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: PID.c
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
#include "queue.h"
#include "emp_type.h"
#include "ADC.h"
#include "spi.h"
#include "PID.h"

/*****************************    Defines    *******************************/


/*****************************   Constants   *******************************/
FP32 kp1 = 0.2;
FP32 ki1 = 1;
FP32 kd1 = 0;

FP32 kp2 = 0.2;
FP32 ki2 = 1;
FP32 kd2 = 0;

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
INT16U convert_voltage_to_duty_cycle(FP32 voltage)
{
    INT16U duty_cycle;
    duty_cycle = (INT16U) (1023.0f/MAXIMUM_VOLTAGE * voltage);
    if (duty_cycle >= 1024)
        while(1);               // duty cycle must be less than 1024
    return duty_cycle;
}

void PID_task(void *pvParameters) {
    FP32 p_term, i_term, d_term;
    FP32 error = 0;
    FP32 last_error = 0;
    FP32 integral_sum1 = 0;
    FP32 integral_sum2 = 0;
    FP32 output = 0;
    FP32 measurement = 0;
    FP32 reference_motor2 = 0;
    FP32 reference_motor1 = 0;
    INT16U duty_cycle_output = 0;
    INT16S adc_y_reference = 0;
    INT16S adc_x_reference = 0;
    INT16S temp_spi_receive;
    INT16U pwm_motor1 = 0, pwm_motor2 = 0, hastighed_modtaget1, hastighed_modtaget2;
    BOOLEAN direction = 0;
    BOOLEAN es = 1;
    INT16S TEMP = 0;


    while (1) {

        spi_update(pwm_motor1, pwm_motor2, &hastighed_modtaget1, &hastighed_modtaget2 );

        //Controller til motor 1 -----------------------------------------------------------------------------

        //adc_y_reference = (INT16S) (adc_get_y() - JOYSTICK_CENTER_VALUE);
        //reference = (FP32) adc_y_reference / VELOCITY_SCALE_Y;

//        reference_motor1 = (adc_get_y() - JOYSTICK_CENTER_VALUE);
//        reference_motor1 = (FP32)reference_motor1 / VELOCITY_SCALE;


        reference_motor1 = 5;

        //measurement = (FP32) spi_receive(); // get current position
        TEMP = hastighed_modtaget1;

        if (TEMP < -1000 )
        {
            measurement = 0;
        }
        else if (TEMP > 1000)
        {
            measurement = 0;
        }
        else
        {
            if (TEMP == 0)
                measurement = 0;
            else
                measurement = 1745.32925/(FP32)TEMP;
        }

        error = reference_motor1 - measurement;

        // proportional term
        p_term = kp1 * error;


        // integral term   (needs anti windup)
        if (es)
        {
            integral_sum1 += error;
            i_term = (ki1 * integral_sum1) * SAMPLING_TIME_MS;
        }

        if (i_term > MAXIMUM_VOLTAGE)
            i_term = MAXIMUM_VOLTAGE;

        if (i_term < -MAXIMUM_VOLTAGE)
            i_term = -MAXIMUM_VOLTAGE;
//
//        // derivative term  (virker ikke måske ikke ved høj sampling frekvens/ støjfyldte målinger (se bandwidth limited derivative))
//        d_term =  (error - last_error) / SAMPLING_TIME_SEK * kd;
//
//        last_error = error; // save error for next derivative calculation

        output = p_term + i_term; // + d_term;



        // restrict output to limits
        if (output > MAXIMUM_VOLTAGE)
        {
            es = 0;
            output = MAXIMUM_VOLTAGE;
        }
        else if (output < -MAXIMUM_VOLTAGE)
        {
            es = 0;
            output = -MAXIMUM_VOLTAGE;
        }
        else
        {
            es = 1;
        }

        // determine direction
        if (output < 0)
        {
            output = -output;
            direction = 1;
        }
        else
        {
            direction = 0;
        }

        duty_cycle_output = convert_voltage_to_duty_cycle(output);

        duty_cycle_output |= 0b100000000000;                // signals that it is motor2
        if (direction == 1)
        {
            duty_cycle_output |= 0b010000000000;
        }

        pwm_motor1 = duty_cycle_output;
        duty_cycle_output &= 0b000000000000;


        //Controller til motor 2 -----------------------------------------------------------------------------
        //adc_y_reference = (INT16S) (adc_get_y() - JOYSTICK_CENTER_VALUE);
       //reference = (FP32) adc_y_reference / VELOCITY_SCALE_Y;

//       reference_motor2 = (adc_get_x() - JOYSTICK_CENTER_VALUE);
//       reference_motor2 = (FP32)reference_motor2 / VELOCITY_SCALE;

        reference_motor2 = 0;
       TEMP = hastighed_modtaget2;

       if (TEMP < -1000 )
       {
           measurement = 0;
       }
       else if (TEMP > 1000)
       {
           measurement = 0;
       }
       else
       {
           if (TEMP == 0)
               measurement = 0;
           else
               measurement = 1745.32925/(FP32)TEMP;
       }

       error = reference_motor2 - measurement;

       // proportional term
       p_term = kp2 * error;


       // integral term   (needs anti windup)
       if (es)
       {
           integral_sum2 += error;
           i_term = (ki2 * integral_sum2) * SAMPLING_TIME_MS;
       }

       if (i_term > MAXIMUM_VOLTAGE)
           i_term = MAXIMUM_VOLTAGE;

       if (i_term < -MAXIMUM_VOLTAGE)
           i_term = -MAXIMUM_VOLTAGE;
//
//        // derivative term  (virker ikke måske ikke ved høj sampling frekvens/ støjfyldte målinger (se bandwidth limited derivative))
//        d_term =  (error - last_error) / SAMPLING_TIME_SEK * kd;
//
//        last_error = error; // save error for next derivative calculation

       output = p_term + i_term; // + d_term;



       // restrict output to limits
       if (output > MAXIMUM_VOLTAGE)
       {
           es = 0;
           output = MAXIMUM_VOLTAGE;
       }
       else if (output < -MAXIMUM_VOLTAGE)
       {
           es = 0;
           output = -MAXIMUM_VOLTAGE;
       }
       else
       {
           es = 1;
       }

       // determine direction
       if (output < 0)
       {
           output = -output;
           direction = 1;
       }
       else
       {
           direction = 0;
       }

       duty_cycle_output = convert_voltage_to_duty_cycle(output);

       duty_cycle_output |= 0b00000000000;                // signals that it is motor2
       if (direction == 1)
       {
           duty_cycle_output |= 0b010000000000;
       }

       pwm_motor2 = duty_cycle_output;
       duty_cycle_output &= 0b000000000000;

       vTaskDelay(pdMS_TO_TICKS(SAMPLING_TIME_MS)); // wait 0.01s (our sampling time)
    }
}




