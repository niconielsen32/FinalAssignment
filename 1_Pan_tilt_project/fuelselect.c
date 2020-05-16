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

/*****************************    Defines    *******************************/
#define LeadFree92 0
#define LeadFree95 1
#define Diesel     2


/*****************************   Constants   *******************************/

FP32 LF92_price = 8.49;
FP32 LF95_price = 8.79;
FP32 Diesel_price = 8.12;

FP32 gas_price = 0;

INT16U gas_type;
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

void select_gas_type(INT16U gastype){

    gas_type = gastype;

    switch(gastype){

       case LeadFree92:
           gas_price = LF92_price;
           break;

       case LeadFree95:
           gas_price = LF95_price;
           break;

       case Diesel:
           gas_price = Diesel_price;
           break;
    }
}


/****************************** End Of Module *******************************/
