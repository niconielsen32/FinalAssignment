/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: file.c
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
* 150322  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "glob_def.h"
#include "file.h"
#include "LCD.h"
#include "keypad.h"

/*****************************    Defines    *******************************/
#define MAX_FILES  8

typedef struct
{
  BOOLEAN (*put)(INT8U);   // Pointer to device put function
  BOOLEAN (*get)(INT8U*);  // Pointer to device get function
} fcb;  //file control block

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
fcb pof[MAX_FILES];             // Pool of files

/*****************************   Functions   *******************************/

BOOLEAN put_file( FILE file, INT8U ch )
{
  if( pof[(int)file].put )
    pof[(int)file].put( ch );
}

BOOLEAN get_file( FILE file, INT8U *pch )
{
  BOOLEAN result = FALSE;

  if( pof[(int)file].get )
    result = pof[(int)file].get( pch );
  return( result );
}


extern INT8U init_files()
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  INT8U i;

  for( i = 0; i < MAX_FILES; i++ )
  {
      pof[i].put = NULL;
      pof[i].get = NULL;
  }
  //pof[COM1].put = uart0_put_q;
  //pof[COM1].get = uart0_get_q;
  pof[COM2].put = wr_ch_LCD;
  pof[COM3].get = get_keyboard;

  return( 1 );
}

/****************************** End Of Module *******************************/












