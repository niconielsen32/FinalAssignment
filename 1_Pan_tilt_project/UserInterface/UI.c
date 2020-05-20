
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
#include "glob_def.h"
#include "FreeRTOS.h"
#include "write.h"
#include "glob_def.h"
#include "frt10/frt10/inc/FreeRTOS.h"
#include "UserInterface/write.h"
#include "data.h"
#include "UI.h"
#include "buttons.h"
#include "file.h"
#include "keypad.h"
#include "string.h"
#include "tmodel.h"
#include "uart0.h"
#include "fuelselect.h"
#include "UserInterface/uart0.h"
#include "pumping.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U temp_char_uart;

FP32 total_sales_lf92 = 0;
FP32 total_sales_lf95 = 0;
FP32 total_sales_diesel = 0;
FP32 total_sum_cash = 0;
FP32 total_sum_card = 0;
FP32 total_op_time = 0;
/*****************************   Functions   *******************************/

void UI_receipt()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : The super loop.
******************************************************************************/
{

    INT8U* ui_product;
    FP32 ui_total_liters;
    FP32 ui_total_price;

    INT8U* newline = "\n";
    INT8U* dkk = " DKK";
    INT8U* product = "Product: ";
    INT8U* total_liters = " - Total liters: ";
    INT8U* total_price = " - Total price: ";

    INT8U* Cash_or_cardNo = " - Acc number: ";
    INT8U card_number[8];
    INT8U* cash = "CASH ";

    INT8U* sClear = "                     ";
    write_string(sClear);


    ui_total_liters = get_total_liters();
    ui_total_price = get_total_amount();

    write_cr();
    write_string(newline);
    write_string(newline);

    if(get_gas_type() == 1){
       ui_product = "LeadFree92";
   } else if(get_gas_type() == 2){
       ui_product = "LeadFree95";
   } else if(get_gas_type() == 3){
       ui_product = "Diesel";
   }

        write_string(product);
        write_string(ui_product);

        write_string(total_liters);
        write_fp32(ui_total_liters);

        write_string(total_price);
        write_fp32(ui_total_price);
        write_string(dkk);


        if(get_payment_type() == CARD){
            write_string(Cash_or_cardNo);
            for(INT8U i = 0; i < 8; i++){
                xQueueReceive(Q_CARDnumber, &card_number[i], 5);
                write_int16u(card_number[i]);
            }
        } else if(get_payment_type() == CASH){
            write_string(Cash_or_cardNo);
            write_string(cash);
        }

}

INT8U get_char_from_uart(){
    return temp_char_uart;
}


void add_to_sales_lf92(FP32 price){
    total_sales_lf92 += price;
}

void add_to_sales_lf95(FP32 price){
    total_sales_lf95 += price;
}

void add_to_sales_diesel(FP32 price){
    total_sales_diesel += price;
}

void add_to_sum_of_cash(FP32 cashPrice){
    total_sum_cash += cashPrice;
}

void add_to_sum_of_card(FP32 cardPrice){
    total_sum_card += cardPrice;
}

void add_to_total_op_time(INT16U seconds){
    total_op_time += seconds;
}


void show_report(){

    INT8U* newline = "\n";
    INT8U* dkk = " DKK ";
    INT8U* minutes = " minutes";
    INT8U* total_sales_lf92_string = "LeadFree92 sales: ";
    INT8U* total_sales_lf95_string = "LeadFree95 sales: ";
    INT8U* total_sales_diesel_string = "Diesel sales: ";
    INT8U* total_sum_cash_string = "Sum of cash purchases: ";
    INT8U* total_sum_card_string = "Sum of card purchases: ";
    INT8U* total_op_time_string = "Total operating time: ";



    if(get_char_from_uart() == 'r'){

        write_string(newline);
        write_string(newline);
        write_string(newline);
        write_cr();
        write_string(total_sales_lf92_string);
        write_fp32(total_sales_lf92);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_sales_lf95_string);
        write_fp32(total_sales_lf95);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_sales_diesel_string);
        write_fp32(total_sales_diesel);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_sum_cash_string);
        write_fp32(total_sum_cash);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_sum_card_string);
        write_fp32(total_sum_card);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_op_time_string);
        write_fp32(total_op_time / 60.0);
        write_string(minutes);
        write_cr();
        write_string(newline);
    }
}


void report_task(void* pvParameters){

    while(1){

        if(uart0_rx_rdy()){
            temp_char_uart = uart0_getc();
            show_report();
        }

    }
}






//                INT8U key = 0;
//                gfprintf(COM2, "%c%cValue: %05u", 0x1B, 0x80, adjusted_value);  // the adjusted value is shown on the first line of the display. this is done outside the state machine so it's displayed all the time
//                switch(ui_state)
//                {
//                case 0:
//                    gfprintf(COM2, "%c%cScale:         ", 0x1B, 0xA8);          // "Scale:" is printed on the second line of the display
//                    key = get_keyboard();                                       // we get a value from the keyboard
//                    if( key >= '0' && key <= '9')                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
//                    {
//                        scale_tmp = key - '0';                                  // the value from the keyboard is given as an ASCII char, so to convert to the actual value we subtract the ASCII-value for 0
//                        ui_state = 1;
//                    }
//                    break;
//                case 1:
//                    gfprintf(COM2, "%c%cOffset:", 0x1B, 0xA8);                  // "Offset:" is printed on the second line of the display
//                    key = get_keyboard();                                       // same procedure as in state 0, but we save the value in off1 since we want it as the first digit of the offset value
//                    if( key >= '0' && key <= '9')
//                    {
//                        gfprintf(COM2, "%c%c%c", 0x1B, 0xC9, key);              // the digit is printed on the second line (after "Offset:")
//                        off1 = (key - '0')*100;                                 // again we subtract the ASCII for 0. we also multiply by 100 since it's the first of the 3 digits
//                        ui_state = 2;
//                    }
//                    break;
//                case 2:
//                    key = get_keyboard();                                       // same procedure for the second digit of the offset value
//                    if( key >= '0' && key <= '9')
//                    {
//                        gfprintf(COM2, "%c%c%c", 0x1B, 0xCA, key);
//                        off2 = (key - '0')*10;
//                        ui_state = 3;
//                    }
//                    break;
//                case 3:
//                    key = get_keyboard();                                       // same procedure for the third digit of the offset value
//                    if( key >= '0' && key <= '9')
//                    {
//                        gfprintf(COM2, "%c%c%c", 0x1B, 0xCB, key);
//                        off3 = (key - '0')*1;
//
//                        if( xSemaphoreTake( xMutex, portMAX_DELAY ))            // we want to change the shared variables so we protect them with a mutex
//                        {
//                            scale = scale_tmp;
//                            offset = off1 + off2 + off3;
//                            xSemaphoreGive(xMutex);
//                        }
//                        ui_state = 0;
//                    }
//                    break;
//                }
//            }

//    INT8U ui_state = 0;
//
//
//    while(1)
//    {
//
//        INT8U key = 0;
//        INT16U type;
//       // gfprintf(COM2, "%c%cChoose payment method", 0x1B, 0x80);  // the adjusted value is shown on the first line of the display. this is done outside the state machine so it's displayed all the time
//        switch(ui_state)
//        {
//        case 0:
//            gfprintf(COM2, "%c%cCash: Press one", 0x1B, 0x80);
//            gfprintf(COM2, "%c%cCard: Press two", 0x1B, 0xA8);          // "Scale:" is printed on the second line of the display
//            key = get_keyboard();                                       // we get a value from the keyboard
//            if( key >= '1' && key <= '2')                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
//            {
//                type = key;                                  // the value from the keyboard is given as an ASCII char, so to convert to the actual value we subtract the ASCII-value for 0
//                ui_state = 1;
//            }
//            break;
//        case 1:
//            gfprintf(COM2, "%c%cYou have method: ", 0x1B, 0x80);                  // "Offset:" is printed on the second line of the display                                      // same procedure as in state 0, but we save the value in off1 since we want it as the first digit of the offset value
//
//            if( type == 49)
//            {
//                gfprintf(COM2, "%c%c     Card      ", 0x1B, 0xA8);              // the digit is printed on the second line (after "Offset:")
//                payment_type = CARD;
//
//
//            }else if ( type == 50){                                 // again we subtract the ASCII for 0. we also multiply by 100 since it's the first of the 3 digits
//                gfprintf(COM2, "%c%c    Cash       ", 0x1B, 0xA8);
//                payment_type = CASH;
//
//            } else{
//                gfprintf(COM2, "%c%c", 0x1B, 0xA8);
//
//            }
//                    //ui_state = 2;
//            break;
//        case 2:
//            key = get_keyboard();                                       // same procedure for the second digit of the offset value
//            if( key >= '0' && key <= '9')
//            {
//                gfprintf(COM2, "%c%c%c", 0x1B, 0xCA, key);
//                off2 = (key - '0')*10;
//                ui_state = 3;
//            }
//            break;
//        case 3:
//            key = get_keyboard();                                       // same procedure for the third digit of the offset value
//            if( key >= '0' && key <= '9')
//            {
//                gfprintf(COM2, "%c%c%c", 0x1B, 0xCB, key);
//                off3 = (key - '0')*1;
//
//                if( xSemaphoreTake( xMutex, portMAX_DELAY ))            // we want to change the shared variables so we protect them with a mutex
//                {
//                    scale = scale_tmp;
//                    offset = off1 + off2 + off3;
//                    xSemaphoreGive(xMutex);
//                }
//                ui_state = 0;
//            }
//            break;
//        }
   // }



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
////    }
//
//    }
//}

/****************************** End Of Module *******************************/



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


       // }

        /****************************** End Of Module *******************************/
