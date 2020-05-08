/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: controller.c
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



/*****************************    Defines    *******************************/
#define SAMPLING_TIME_MS 10
#define VELOCITY_SCALE_Y 150.0f
#define JOYSTICK_CENTER_VALUE 2048
#define MAXIMUM_VOLTAGE 12.0f

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT16S tempx, tempy, Xspeed, Yspeed;
INT16U modtaget;
/*****************************   Functions   *******************************/


//INT16U convert_voltage_to_duty_cycle(FP32 voltage)
//{
//    INT16U duty_cycle;
//    duty_cycle = (INT16U) (1023.0f/MAXIMUM_VOLTAGE * voltage);
//    if (duty_cycle >= 1024)
//        while(1);               // duty cycle must be less than 1024
//    return duty_cycle;
//}


void controller_task(void *pvParameters)
{
    FP32 u_0 = 0;     // current input
    FP32 y_0 = 0;     // current output
    FP32 measurement = 0;
    FP32 reference = 0;
    FP32 speed = 0;
    BOOLEAN direction = 0;

    INT16U duty_cycle_output = 0;
    INT16S adc_y_reference = 0;

    FP32 y_1 = 0; // output one time step ago
    FP32 u_1 = 0; // input one time step ago
    while(1)
    {

//        tempx = (INT16S)adc_get_x() - 2048;  //for at få 11 bit 4096
//        tempy =   (INT16S)adc_get_y() - 2048;  //for at få 11 bit
//
//          if ((tempx > -150 && tempx < 150) && (tempy > -150 && tempy < 150) )
//              spi_send(0);
//          else
//          {
//              if (tempx < -150)
//              {
//                  Xspeed = (~(tempx)) / 2; //Negeret for at få tallet positivt
//                  Xspeed |= 0b0000000000000000;
//                  spi_send(Xspeed); //0b000000000000
//              }
//              else if (tempx > 150 )
//              {
//                  Xspeed = tempx / 2 ;//+ 1024;
//                  Xspeed |= 0b0000010000000000;
//                  spi_send(Xspeed); //0b01
//              }
//
//              if (tempy < -150)
//              {
//                  Yspeed = (~(tempy)) / 2;// + 2048;
//                  Yspeed |= 0b0000100000000000;
//                  spi_send(Yspeed); //0b100000000000
//              }
//              else if (tempy > 150 )
//              {
//                  Yspeed = tempy / 2; // + 3072;
//                  Yspeed |= 0b0000110000000000;
//                  spi_send(Yspeed); //0b11
//              }
//          }
//
//          modtaget = spi_receive();


        adc_y_reference = (INT16S) (adc_get_y() - JOYSTICK_CENTER_VALUE);

        reference = (FP32) adc_y_reference / VELOCITY_SCALE_Y;
        //measurement = (FP32) spi_receive(); // Receives time in 100'th of a ms pr degree
        measurement = 0.0174532925f / ((FP32) spi_receive() / 100) * 1000;  // To get the speed in rad/s.
        // measurement needs to be updated from SPI (maybe derivative for speed?)

        u_0 = reference - measurement; // error
        y_0 = 17.68f * u_0 - 17.32 * u_1 - y_1; // controller

        u_1 = u_0; // update values for next iteration
        y_1 = y_0;


        if (y_0 > MAXIMUM_VOLTAGE)
            y_0 = MAXIMUM_VOLTAGE;

        if (y_0 < -MAXIMUM_VOLTAGE)
            y_0 = -MAXIMUM_VOLTAGE;

        if (y_0 < 0)
        {
            y_0 = -y_0;
            direction = 0;
        }
        else
        {
            direction = 1;
        }

        duty_cycle_output = convert_voltage_to_duty_cycle(y_0);
        duty_cycle_output |= 0b100000000000;                // signals that it is motor2
        if (direction == 1)
        {
            duty_cycle_output |= 0b010000000000;
        }
        // write y_0 to SPI after conversion from volt to pwm...



        spi_send(duty_cycle_output);
        vTaskDelay(pdMS_TO_TICKS(SAMPLING_TIME_MS)); // wait 0.01s (our sampling time)

    }
}


/****************************** End Of Module *******************************/




