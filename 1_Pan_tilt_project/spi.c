/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: spi.c
*
* PROJECT....: Semesterprojekt 4
*
* DESCRIPTION: See module specification file (.h-file).
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
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "spi.h"
#include "ADC.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "glob_def.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
QueueHandle_t Q_SPI_READ = NULL;
QueueHandle_t Q_SPI_SEND = NULL;


/*****************************   Functions   *******************************/

void init_spi(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{
    int dummy;
    //Enable SSI module 0
    SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;

    dummy = SYSCTL_RCGCSSI_R;


    SSI0_CR1_R = 0x00; // Synchronous serial port disable (burde skrive ~(0x02) i stedet for 0x00)
    SSI0_CR1_R &= ~(0x04); // Sæt tiva som master
    SSI0_CC_R = SSI_CC_CS_SYSPLL; // Sæt til systemclock
    SSI0_CPSR_R = 0x02; // Prescaler med en devisor på 2


    // Sætter serial clock rate til 39, clock phase second edge, clock polarity steady state high, SPI, datasize 12 bit.
    SSI0_CR0_R = SSI_CR0_FRF_MOTO | SSI_CR0_DSS_16; // SSI_CR0_SPH | SSI_CR0_SPO |  0x2700
    SSI0_CR1_R = SSI_CR1_SSE; // Synchronous serial port enable.

    Q_SPI_READ = xQueueCreate(1, sizeof(INT16U));
    Q_SPI_SEND = xQueueCreate(1, sizeof(INT16U));
}

void spi_task(void * pvParameters)

{
    INT16U data;
    INT16U receive;

    while(1)
    {
      if ( xQueueReceive(Q_SPI_SEND, &data, 0) )   //check om der er noget på køen
      {
          SSI0_DR_R = data;              //putting the byte to send from SSI
          while ((SSI0_SR_R & (1 << 0)) == 0)
          {
              vTaskDelay(1); //waiting for transmission to be done
          }
      }


      while((SSI0_SR_R & (1<<3)))  //vent til Receive FIFO er tomt
      {
          receive = SSI0_DR_R;
          vTaskDelay(1);
      }
      xQueueOverwrite( Q_SPI_READ , &receive);
      vTaskDelay( pdMS_TO_TICKS(5) );
    }
}

void spi_send(INT16U data)
{
    xQueueOverwrite(Q_SPI_SEND , &data);
}

INT16U spi_peek_send_queue()
{
    INT16U data;
    xQueuePeek(Q_SPI_SEND, &data, 0);
    return data;
}

INT16U spi_receive()
{
    INT16S data;
    xQueuePeek(Q_SPI_READ, &data, 0);
    return data;
}

void spi_update(INT16U motor1_pwm, INT16U motor2_pwm, INT16S* encoder1, INT16S* encoder2)
{
    SSI0_DR_R = motor1_pwm;              //putting the byte to send from SSI
    while ((SSI0_SR_R & (1 << 0)) == 0);

    while((SSI0_SR_R & (1<<3)))  //vent til Receive FIFO er tomt
    {
        *encoder1 = SSI0_DR_R;
    }

    SSI0_DR_R = motor2_pwm;              //putting the byte to send from SSI
    while ((SSI0_SR_R & (1 << 0)) == 0);

    while((SSI0_SR_R & (1<<3)))  //vent til Receive FIFO er tomt
    {
        *encoder2 = SSI0_DR_R;
    }

}



/****************************** End Of Module *******************************/
