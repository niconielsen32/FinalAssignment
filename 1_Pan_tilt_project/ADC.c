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
#include "ADC.h"
#include "glob_def.h"
#include "FreeRTOS.h"
#include "queue.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

QueueHandle_t Q_ADC_X;
QueueHandle_t Q_ADC_Y;


/*****************************   Functions   *******************************/



void sleep(int time)
{
    NVIC_ST_CTRL_R = 0;             /* disable SysTick during setup */
    NVIC_ST_RELOAD_R = time*80000-1;    /* Reload Value goes here */
    NVIC_ST_CTRL_R |= 0x5;          /* enable SysTick with core clock */
    while( (NVIC_ST_CTRL_R & (1<<16) ) == 0);                      /* Monitoring bit 16 to be set */
    NVIC_ST_CTRL_R = 0;             /* Disabling SysTick Timer */
}

void init_ADC(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{

    SYSCTL_RCGCADC_R = SYSCTL_RCGCADC_R0 | SYSCTL_RCGCADC_R1;      // Enable ADC

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;    // Enable GPIO port E;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;

    GPIO_PORTE_AFSEL_R |= 0x0C;                 // Set AIN0 og AIN1 på PE3 og PE2 til AF

    GPIO_PORTE_DEN_R &=  0x0C;                  // Clear AIN0 og AIN1 fra digital

    GPIO_PORTE_AMSEL_R |= 0x0C;                 // Set AIN0 og AIN1 til analog


    // Til knappen
    GPIO_PORTE_DIR_R &= 0x02;                   //pin pe1 er input

    GPIO_PORTE_AMSEL_R &=

    GPIO_PORTE_PUR_R |= 0x02;                   // Pull-up modstand

    GPIO_PORTE_DEN_R |= 0x02;                   // Digital pin

    GPIO_PORTE_DATA_R = 0;



    ADC0_ACTSS_R &= ADC_ACTSS_ASEN3;            //Clear Sample sequencer
    ADC0_EMUX_R |=ADC_EMUX_EM3_M;               // set trigger til sample sequencer
    ADC0_SSMUX3_R |= ADC_SSMUX3_MUX0_S;
    ADC0_SSCTL3_R = ADC_SSCTL3_END0 | ADC_SSCTL3_IE0;
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN3;            // Enable sample sequencer

    ADC1_ACTSS_R &= ADC_ACTSS_ASEN3;            //Clear Sample sequencer
    ADC1_EMUX_R |=ADC_EMUX_EM3_M;               // set trigger til sample sequencer
    ADC1_SSMUX3_R = 1;
    ADC1_SSCTL3_R = ADC_SSCTL3_END0 | ADC_SSCTL3_IE0;
    ADC1_ACTSS_R |= ADC_ACTSS_ASEN3;            // Enable sample sequencer

    Q_ADC_Y = xQueueCreate(1, sizeof(INT16U));
    Q_ADC_X = xQueueCreate(1, sizeof(INT16U));
}



void adc_read_task( void * pvParameters )
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/

{
    init_ADC();

    while(1)
    {
        INT16S resultx;
        ADC0_PSSI_R |= ADC_PSSI_SS3;                // Start conversion
        while ((ADC0_RIS_R & 0x08) == 0);           // wait for conversion
        resultx = ADC0_SSFIFO3_R;                    // Save result

        if (resultx > 1898 && resultx < 2198)
                    resultx = 2048;

        ADC0_ISC_R = 0x08;                          // Clear completion flag
        xQueueOverwrite( Q_ADC_X, &resultx );


        INT16S resulty;
        ADC1_PSSI_R |= ADC_PSSI_SS3;                // Start conversion
        while ((ADC1_RIS_R & 0x08) == 0);           // wait for conversion
        resulty = ADC1_SSFIFO3_R;                    // Save result

        if (resulty > 1898 && resulty < 2198)
            resulty = 2048;

        ADC1_ISC_R = 0x08;                          // Clear completion flag
        xQueueOverwrite( Q_ADC_Y, &resulty);

        vTaskDelay(pdMS_TO_TICKS(10));

    }

}

INT16U adc_get_x()
{
    INT16U data;
    xQueuePeek( Q_ADC_X, &data, (TickType_t) 0 );
    return data;
}

INT16U adc_get_y()
{
    INT16U data;
    xQueuePeek(Q_ADC_Y, &data,  0);
    return data;
}





/****************************** End Of Module *******************************/
