/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: pumping.c
*
* PROJECT....: EMP
*
*  Definements of variable types.
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
#include <math.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "pumping.h"
#include "buttons.h"
#include "flowmeter.h"
#include "fuelselect.h"
#include "payment.h"
#include "UserInterface/write.h"
#include "timer.h"
#include "LCD.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/
static INT16U  pumping_state = no_pumping;

FP32 total_amount;
FP32 gas_price_temp;
INT16U cur_button_state;
INT16U type_of_payment;
FP32 out_of_cash_cal;
FP32 total_cash_temp;
INT16U total_pulses_temp = 0;
FP32 total_liters = 0;
FP32 pulses_pr_liter = 512.0;
FP32 SEC3_reduced = 0.15;

INT16U seconds_lever = 0;
INT16U seconds = 0;
INT16U total_pumping_time = 0;

INT16U running_pulses;
FP32 running_liters;
FP32 price_one_liter;
FP32 running_total_price;
FP32 digi_cash;

char liter[7];
char dollar[7];
char Price_per_liter[7];

BOOLEAN pumping_stopped;
BOOLEAN reduced_last;

/*****************************   Functions   *******************************/

//This is for string convertion
long int x_to_the_n (int x,int n)
{
    int i; /* Variable used in loop counter */
    int number = 1;

    for (i = 0; i < n; ++i)
        number *= x;

    return(number);
}

void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * x_to_the_n(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
//************************************************************************
INT8U get_seconds_lever(){
    return seconds_lever;
}
BOOLEAN set_reduced_last(BOOLEAN reduced){
    reduced_last = reduced;
}

BOOLEAN get_pumping_stopped(){
    return pumping_stopped;
}

void set_pumping_stopped(BOOLEAN pumping_bool){
    pumping_stopped = pumping_bool;
}

INT16U get_pumping_state(){
    return pumping_state;
}

FP32 get_total_liters(){
    return total_liters;
}

FP32 get_total_amount(){
    return total_amount;
}



void pumping_timer_callback(TimerHandle_t timer) {
    seconds--;
}

void total_pumping_time_callback(TimerHandle_t timer) {
    total_pumping_time++;
}

void lever_timer_callback(TimerHandle_t timer){
    seconds_lever++;
}


void display_pumping(){

    running_pulses = get_total_pulses();
    running_liters = running_pulses / 512.0; // skal skrives ud på LCD et andet sted når display_pumping_lcd == true

    price_one_liter = get_gas_price(); // skal skrives ud på LCD et andet sted når display_pumping_lcd == true

    running_total_price = running_liters * price_one_liter; // skal skrives ud på LCD et andet sted når display_pumping_lcd == true

    ftoa(running_liters, liter, 2);
    ftoa(running_total_price, dollar, 2);
    ftoa(price_one_liter, Price_per_liter, 2);
//    write_string(" Pulses: ");
//    write_int16u(get_total_pulses());


   //write_string(" ");
   //write_fp32(running_liters);

   //write_string(" ");
   //write_fp32(price_one_liter);
   gfprintf(COM2, "%c%c L    PPL   Dkk ", 0x1B, 0x80);
   gfprintf(COM2, "%c%c%s %s %s      ", 0x1B, 0xA8, liter, Price_per_liter, dollar);

   write_string(" ");
   write_fp32(running_total_price);

}


void pumping_task(void* pvParameters){

    TickType_t last_unblock_pumping;
    last_unblock_pumping = xTaskGetTickCount();


    while(1){

//write_int16u(total_pumping_time);
//write_string(" ");

        cur_button_state = get_button_state();
        gas_price_temp = get_gas_price();
        total_cash_temp = get_total_cash_from_digi();
        out_of_cash_cal = gas_price_temp * SEC3_reduced;



            if(get_pumping_stopped()){
                xTimerStop(timer_total_pumping, 0);
                total_pumping_time = 0;
                total_pulses_temp = get_total_pulses();
                total_liters = total_pulses_temp / pulses_pr_liter;
                total_amount = total_liters * gas_price_temp;
                pumping_state = no_pumping;
                write_string(" Terminated ");
                terminate_session();
            } else {

                switch(pumping_state)
                  {
                    case no_pumping:

                        GPIO_PORTF_DATA_R = 0x02; //red
                        seconds = 0;
                        //write_string("no ");
                        if(cur_button_state == nozzle_removal){
                            set_total_pulses(0);
                            //write_int16u(get_total_pulses());
                            xTimerStart(timer_total_pumping, 0);
                            pumping_state = pumping_idle;
                        }

                        break;

                    case pumping_idle:

                        GPIO_PORTF_DATA_R = 0x02; //red
                        //write_string("idle ");
                        if(cur_button_state == lever_depressed){
                            xTimerStart(timer_pumping, 0);
                            seconds = 2;
                            pumping_state = pumping_reduced_2sec;
                        }

                        break;

                    case pumping_reduced_2sec:

                        GPIO_PORTF_DATA_R = 0x04; //yellow
                        //write_string("start ");

                        display_pumping();

                        if(seconds == 0){
                            xTimerStop(timer_pumping, 0);
                            pumping_state = pumping_regular;
                        }

                        if(cur_button_state == lever_released){
                           xTimerStart(timer_lever, 0);
                           pumping_state = pumping_reduced_1sec;
                        }

                        break;

                    case pumping_regular:

                        GPIO_PORTF_DATA_R = 0x08; //green
                        //write_string("regu ");

                        display_pumping();


                        if(cur_button_state == lever_released){
                            xTimerStart(timer_lever, 0);
                            xTimerStart(timer_pumping, 0);
                            seconds = 1;
                            pumping_state = pumping_reduced_1sec;
                        }


                        if((get_payment_type() == CASH) && ((total_cash_temp - running_total_price ) <= out_of_cash_cal)){ // total amont skal være et løbende beløb
                            reduced_last = TRUE;
                            write_string(" Almost there ");
                            pumping_state = pumping_reduced_1sec;
                            //bool reduced3sec = true
                            //go to pumping_stop
                            //reduced speed for 3 sec
                        }

                        break;

                    case pumping_reduced_1sec:
                        write_string(" Reduced ");
                        GPIO_PORTF_DATA_R = 0x04; //yellow
                        write_int16u(running_liters);
                        display_pumping();

                        if (reduced_last){
                            if(running_total_price >= total_cash_temp){
                              // total_pulses_temp = get_total_pulses();
                                 //write_string("price is reached!");
                                 pumping_state = pumping_stop;
                                 //set_pumping_stopped(TRUE);
                             }
                        } else if (cur_button_state == lever_depressed){
                            seconds_lever = 0;
                            xTimerStop(timer_lever, 0);
                            pumping_state = pumping_regular;
                        } else if(seconds == 0){
                           xTimerStop(timer_pumping, 0);
                           pumping_state = pumping_stop;
                        }

                        break;

                    case pumping_stop:
                        if(seconds_lever == 15){
                            seconds_lever = 0;
                            write_string("15sec timeout");
                            xTimerStop(timer_lever, 0);
                            set_pumping_stopped(TRUE);
                        }

                        if((seconds_lever == 5) && (get_payment_type() == CARD)){
                            seconds_lever = 0;
                            write_string("5sec timeout");
                            xTimerStop(timer_lever, 0);
                            set_pumping_stopped(TRUE);
                        }

                        write_string(" Stop ");
                        if(!(GPIO_PORTF_DATA_R & 0x10)){ //sw1 pressed
                            xTimerStop(timer_pumping, 0);
                            set_pumping_stopped(TRUE);
                        }

                        break;
                  }
          }

       vTaskDelayUntil(&last_unblock_pumping, pdMS_TO_TICKS(1000));
    }
}

/****************************** End Of Module *******************************/

