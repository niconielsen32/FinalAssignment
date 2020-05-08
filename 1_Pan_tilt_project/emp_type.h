/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: emp_type.h
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

#ifndef _EMP_TYPE_H
  #define _EMP_TYPE_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

typedef unsigned char           BOOLEAN;
typedef unsigned char           INT8U;     /* Unsigned  8 bit quantity              */
typedef signed   char           INT8S;     /* Signed    8 bit quantity              */
typedef unsigned short          INT16U;    /* Unsigned 16 bit quantity              */
typedef signed   short          INT16S;    /* Signed   16 bit quantity              */
typedef unsigned long           INT32U;    /* Unsigned 32 bit quantity              */
typedef signed   long           INT32S;    /* Signed   32 bit quantity              */
typedef unsigned long long      INT64U;    /* Unsigned 64 bit quantity              */
typedef signed   long long      INT64S;    /* Signed   64 bit quantity              */
typedef float                   FP32;      /* Single precision floating point       */
typedef double                  FP64;      /* Double precision floating point       */

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/
#endif
