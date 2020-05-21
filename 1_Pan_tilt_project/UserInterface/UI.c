
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
#include "UserInterface/write.h"
#include "UI.h"
#include "uart0.h"
#include "fuelselect.h"
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

    if(get_payment_type() == CASH){
        ui_total_price = get_total_cash_temp();
    } else {
        ui_total_price = get_total_amount();
    }


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
        write_fp32_one_digit(ui_total_liters / 100.0);


        write_string(total_price);
        write_fp32_one_digit(ui_total_price / 100.0);
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

        write_string(newline);

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
        write_fp32_one_digit(total_sales_lf92 / 100.0);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_sales_lf95_string);
        write_fp32_one_digit(total_sales_lf95 / 100.0);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_sales_diesel_string);
        write_fp32_one_digit(total_sales_diesel / 100.0);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_sum_cash_string);
        write_fp32_one_digit(total_sum_cash / 100.0);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_sum_card_string);
        write_fp32_one_digit(total_sum_card / 100.0);
        write_string(dkk);
        write_cr();
        write_string(newline);

        write_string(total_op_time_string);
        write_fp32_one_digit((total_op_time / 60.0) / 100.0);
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


