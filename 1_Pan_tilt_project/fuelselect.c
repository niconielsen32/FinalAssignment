/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: fuelselect.c
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
#include "emp_type.h"
#include "glob_def.h"
#include "fuelselect.h"
#include "payment.h"
#include "LCD.h"
#include "file.h"

/*****************************    Defines    *******************************/
#define LeadFree92 1
#define LeadFree95 2
#define Diesel     3


/*****************************   Constants   *******************************/

FP32 LF92_price = 8.49;
FP32 LF95_price = 8.79;
FP32 Diesel_price = 8.12;

FP32 gas_price = 0;


INT16U gas_type;
INT8U gas_state = 0;
INT8U gastype;
BOOLEAN fuelselect;
/*****************************   Functions   *******************************/
FP32 get_gas_price(){
    return gas_price;
}

INT16U get_gas_type(){
    return gas_type;
}

void set_gas_price(INT16U gastype, FP32 gasprice){

    switch(gastype){

           case LeadFree92:
               LF92_price = gasprice;
               break;

           case LeadFree95:
               LF95_price = gasprice;
               break;

           case Diesel:
               Diesel_price = gasprice;
               break;
        }
}

BOOLEAN get_fuelselect_complete(){
    return fuelselect;
}

void select_gas_type(INT16U gastype){

    gas_type = gastype;

    switch(gastype){

       case LeadFree92:
           gfprintf(COM2, "%c%c   LeadFree92   ", 0x1B, 0xA8);
           gas_price = LF92_price;
           break;

       case LeadFree95:
           gfprintf(COM2, "%c%c   LeadFree95   ", 0x1B, 0xA8);
           gas_price = LF95_price;
           break;

       case Diesel:
           gfprintf(COM2, "%c%c     Diesel      ", 0x1B, 0xA8);
           gas_price = Diesel_price;
           break;
    }
}

void fuelselect_task(void* pvParameters){

    while(1)
    {
        INT8U key = 0;
       // gfprintf(COM2, "%c%cChoose payment method", 0x1B, 0x80);  // the adjusted value is shown on the first line of the display. this is done outside the state machine so it's displayed all the time
        if(get_paytype_complete()){
            switch(gas_state)
            {

            case 0:
                gfprintf(COM2, "%c%cLF92: LF95:  D: ", 0x1B, 0x80);
                gfprintf(COM2, "%c%c  1     2    3  ", 0x1B, 0xA8);          // "Scale:" is printed on the second line of the display
                //gfprintf(COM2, "%c%cD: Press 3", 0x1B, 0xA8);
                key = get_keyboard();                                       // we get a value from the keyboard
                if( key >= '1' && key <= '3'){
                    gastype = key - '0';
                    //write_int16u(gastype);// the value from the keyboard is given as an ASCII char, so to convert to the actual value we subtract the ASCII-value for 0
                    gas_state = 1;
                }
                break;

            case 1:
                gfprintf(COM2, "%c%cYou have chosen: ", 0x1B, 0x80);
                select_gas_type(gastype);
                fuelselect = TRUE;
                break;
            }
        }

        vTaskDelay(  100 / portTICK_PERIOD_MS);
    }
}

/****************************** End Of Module *******************************/
