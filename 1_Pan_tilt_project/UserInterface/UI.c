
/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: gpio.c
*
* PROJECT....: EMP
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
#include "emp_type.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include "glob_def.h"
#include "FreeRTOS.h"
#include "write.h"
#include "data.h"
#include "UI.h"
#include "buttons.h"
#include "file.h"
#include "keypad.h"
#include "string.h"
#include "tmodel.h"
#include "uart0.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/


/*****************************   Functions   *******************************/



void UI_task(void * pvParameters)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{


    INT8U ui_state = 0;
    INT16U off1;                                                        // off1-3 are used for storing the 3 digits of the offset temporarily
    INT8U off2;
    INT8U off3;
    INT8U scale_tmp;                                                    // scale_tmp stores the scale value temporarily
    scale = 1;                                                          // we initialize the scale and offset values
    offset = 0;

    INT16U temp = 0;

    while(1)
    {
//        if(uart0_rx_rdy()){
//            temp = uart0_getc();
//        }
//        if(temp){
//            write_string("modtaget ");
//            temp = 0;
//        }


                INT8U key = 0;
                gfprintf(COM2, "%c%cValue: %05u", 0x1B, 0x80, adjusted_value);  // the adjusted value is shown on the first line of the display. this is done outside the state machine so it's displayed all the time
                switch(ui_state)
                {
                case 0:
                    gfprintf(COM2, "%c%cScale:         ", 0x1B, 0xA8);          // "Scale:" is printed on the second line of the display
                    key = get_keyboard();                                       // we get a value from the keyboard
                    if( key >= '0' && key <= '9')                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
                    {
                        scale_tmp = key - '0';                                  // the value from the keyboard is given as an ASCII char, so to convert to the actual value we subtract the ASCII-value for 0
                        ui_state = 1;
                    }
                    break;
                case 1:
                    gfprintf(COM2, "%c%cOffset:", 0x1B, 0xA8);                  // "Offset:" is printed on the second line of the display
                    key = get_keyboard();                                       // same procedure as in state 0, but we save the value in off1 since we want it as the first digit of the offset value
                    if( key >= '0' && key <= '9')
                    {
                        gfprintf(COM2, "%c%c%c", 0x1B, 0xC9, key);              // the digit is printed on the second line (after "Offset:")
                        off1 = (key - '0')*100;                                 // again we subtract the ASCII for 0. we also multiply by 100 since it's the first of the 3 digits
                        ui_state = 2;
                    }
                    break;
                case 2:
                    key = get_keyboard();                                       // same procedure for the second digit of the offset value
                    if( key >= '0' && key <= '9')
                    {
                        gfprintf(COM2, "%c%c%c", 0x1B, 0xCA, key);
                        off2 = (key - '0')*10;
                        ui_state = 3;
                    }
                    break;
                case 3:
                    key = get_keyboard();                                       // same procedure for the third digit of the offset value
                    if( key >= '0' && key <= '9')
                    {
                        gfprintf(COM2, "%c%c%c", 0x1B, 0xCB, key);
                        off3 = (key - '0')*1;

                        if( xSemaphoreTake( xMutex, portMAX_DELAY ))            // we want to change the shared variables so we protect them with a mutex
                        {
                            scale = scale_tmp;
                            offset = off1 + off2 + off3;
                            xSemaphoreGive(xMutex);
                        }
                        ui_state = 0;
                    }
                    break;
                }
            }



        //    INT8U button;
        //
        //
        //    while(1)
        //    {
        //        button = get_button_state;
        //
        //        // test for buttons working
        //
        //               if(!(GPIO_PORTF_DATA_R & 0x10)){ //sw1
        //                   //GPIO_PORTF_DATA_R = 0x08; //put it on the green LED
        //                   button = "button1";
        //               } else if(!(GPIO_PORTF_DATA_R & 0x01)){ //sw2
        //                   //GPIO_PORTF_DATA_R = 0x02; //put it on the RED LED
        //                   button = "button2";
        //               } else {
        //                   //GPIO_PORTF_DATA_R = 0x00;
        //                   button = "No button";
        //               }
        //
        //
        ////        write_string(button);
        ////        write_character(' ');
        //
        ////          write_character(button);
        ////          write_character(' ');
        ////        // write output
        ////        // Motor 1
        ////        write_string(sMotor1);
        ////        write_string(sVel);
        ////        write_fp32(leadfree92);
        ////        write_character(' ');
        //
        ////        write_cr();
        //
        //
        //        vTaskDelay(pdMS_TO_TICKS(100));
        //    }


        }

        /****************************** End Of Module *******************************/
