/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: digiswitch.c
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
#include "strings.h"
#include <stdio.h>
#include <stdlib.h>

#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "write.h"
#include "uart0.h"
#include "UI.h"
#include "payment.h"
#include "pumping.h"
/*****************************    Defines    *******************************/

/***********************   Constants & variables  **************************/

enum functions{
    change,
    LF92_state,
    LF95_state,
    Diesel_state,
    Display_state,
    report_state,
    try
};

INT8U number_iterate;

char fuel_LF92[5] ="08.49";
FP32 fuel_LF92_f;

char fuel_LF95[5] = "08.79";
FP32 fuel_LF95_f;

char fuel_Diesel[5] = "08.12";
FP32 fuel_Diesel_f;

enum functions func = change;


/*****************************   Functions   *******************************/

void wr_ch_UART(char character)
{
    while(!(uart0_tx_rdy())){}      //wait for transfer
    uart0_putc(character);
}

void wr_str_UART(char* txt){
    int i = 0;
    for (i ; i < strlen(txt); i++){
        wr_ch_UART(txt[i]);
    }
}

FP32 get_fuel_LF92(){
    fuel_LF92_f = atof(fuel_LF92);
    return fuel_LF92_f;
}

FP32 get_fuel_LF95(){
    fuel_LF95_f = atof(fuel_LF95);
    return fuel_LF95_f;
}

FP32 get_fuel_Diesel(){
    fuel_Diesel_f = atof(fuel_Diesel);
    return fuel_Diesel_f;
}

void change_price_task(void *pvParameters){
    char activate_change[100];
    INT8U letter_iterate;


    while(1)
    {
        if (get_change_price()){
        if(uart0_rx_rdy()){                   //check if anything is entered in UART

            char round_char = uart0_getc();

            wr_ch_UART(round_char);            //Display entered character in UART
            if(round_char != 18){              //No backspace

                if(round_char == 13){          //Check if the 'enter' key is pressed
                    switch(func){
                    case change:
//
                        if(strcmp("change", activate_change) == 0){
                            wr_str_UART("\n*************** Change fuel prices *******************\n");
                            func = LF92_state;
                        }
                        //else if(strcmp("report", activate_change) == 0){
//                            wr_str_UART("\n*************** Report of the day *******************\n");
//                            func = report_state;
//                        }
                        break;
//
                    case LF92_state:
                        wr_str_UART("\n");
                        wr_str_UART("LeadFree 92 new price: ");
                        func = LF95_state;
                        break;

                    case LF95_state:
                        //write_fp32(fuel_LF92_f);
                        for (int i = 0; i < 5; i++){
                              fuel_LF92[i] = activate_change[i];
                        }

                        wr_str_UART("\n");
                        wr_str_UART("LeadFree 95 new price: ");

                        //write_string(activate_change);
                        ///wr_str_UART("\n");
                        func = Diesel_state;
                        break;

                    case Diesel_state:
                        for (int i = 0; i < 5; i++){
                              fuel_LF95[i] = activate_change[i];
                        }

                        wr_str_UART("\n");
                        wr_str_UART("Diesel new price: ");

                        func = Display_state;
                        break;

                    case Display_state:
                        for (int i = 0; i < 5; i++){
                              fuel_Diesel[i] = activate_change[i];
                        }

                        wr_str_UART("\n");
                        func = try;
                        break;

                    case report_state:
                      //show_report();
                        func = try;
                        break;
                    case try:
                        write_string("reset");
                        func = change;
                        break;
                    }

                    memset(activate_change, 0, sizeof(activate_change));                      // Clear text
                    letter_iterate = 0;
//
                    }
//                                                      // Point to first character in text again
//
//
                else
                {
                    activate_change[letter_iterate++] = round_char;
                    activate_change[letter_iterate] = '\0';
                }

            }
        }

        }

        }

      //  vTaskDelay(10);

}
/****************************** End Of Module *******************************/

