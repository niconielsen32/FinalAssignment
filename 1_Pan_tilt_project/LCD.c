/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: leds.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
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
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "LCD.h"
#include "glob_def.h"
#include "file.h"
#include "string.h"
#include "keypad.h"
#include "UserInterface/write.h"
#include "queue.h"
#include "payment.h"



/*****************************    Defines    *******************************/

#define QUEUE_LEN   128

enum LCD_states
{
  LCD_POWER_UP,
  LCD_INIT,
  LCD_READY,
  LCD_ESC_RECEIVED,
};

/*****************************   Constants   *******************************/
const INT8U LCD_init_sequense[]=
{
  0x30,     // Reset
  0x30,     // Reset
  0x30,     // Reset
  0x20,     // Set 4bit interface
  0x28,     // 2 lines Display
  0x0C,     // Display ON, Cursor OFF, Blink OFF
  0x06,     // Cursor Increment
  0x01,     // Clear Display
  0x02,         // Home
  0xFF      // stop
};

/*****************************   Variables   *******************************/
//INT8U LCD_buf[QUEUE_LEN];
//INT8U LCD_buf_head = 0;
//INT8U LCD_buf_tail = 0;
//INT8U LCD_buf_len  = 0;

enum LCD_states LCD_state = LCD_POWER_UP;
INT8U LCD_init;
INT16U payment_type;

BOOLEAN card_number_entered;
BOOLEAN pin_code_entered;
BOOLEAN cash_selected;

BOOLEAN paytype_complete;
INT8U ui_state = 0;
INT8U order = 0;
INT8U key = 0;
INT8U card_cif;
INT8U pin_cif;
//INT8U card_try;        //<-- If pin is wrong 3 times
INT16U type;

/*****************************   Functions   *******************************/

INT16U get_pay_type()
{
    INT8U key = 0;

    gfprintf(COM2, "%c%cCash: Press one", 0x1B, 0x80);
    gfprintf(COM2, "%c%cCard: Press Two", 0x1B, 0xA8);

    key = get_keyboard();                                      // we get a value from the keyboard
    if( key == 1)                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
      {
       return CASH;
      }
    else if (key == 2)
      {
       return CARD;
      }
}

BOOLEAN get_card_number_entered(){
    return card_number_entered;
}
BOOLEAN get_paytype_complete(){
    //write_int16u(paytype_complete);
    return paytype_complete;
}

void select_pay_type_task(void* pvParameters){

    while(1)
    {
       // gfprintf(COM2, "%c%cChoose payment method", 0x1B, 0x80);  // the adjusted value is shown on the first line of the display. this is done outside the state machine so it's displayed all the time
        if(!paytype_complete){

        switch(ui_state)
        {

        case 0:
            gfprintf(COM2, "%c%cCard: Press one", 0x1B, 0x80);
            gfprintf(COM2, "%c%cCash: Press two", 0x1B, 0xA8);          // "Scale:" is printed on the second line of the display
            key = get_keyboard();                                       // we get a value from the keyboard
            if( key >= '1' && key <= '2')                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
            {
                type = key - '0';
                //write_int16u(type);// the value from the keyboard is given as an ASCII char, so to convert to the actual value we subtract the ASCII-value for 0
                ui_state = 1;
            }
            break;

        case 1:
            gfprintf(COM2, "%c%cYou have method: ", 0x1B, 0x80);                  // "Offset:" is printed on the second line of the display                                      // same procedure as in state 0, but we save the value in off1 since we want it as the first digit of the offset value

            if( type == CARD)
            {
                //card_try = 3;
                gfprintf(COM2, "%c%c     Card      ", 0x1B, 0xA8);              // the digit is printed on the second line (after "Offset:")
                payment_type = CARD;
                ui_state = 2;

            }else if ( type == CASH){                                 // again we subtract the ASCII for 0. we also multiply by 100 since it's the first of the 3 digits
                gfprintf(COM2, "%c%c     Cash      ", 0x1B, 0xA8);
                payment_type = CASH;
               // write_string("cashaha");
                cash_selected = TRUE;
                ui_state = 3;
                break;

            } else{
                gfprintf(COM2, "%c%c", 0x1B, 0xA8);

            }
            break;

        case 2:

             switch(order)
             {

             case 0:
                 gfprintf(COM2, "%c%cCard number:    ", 0x1B, 0x80);
                 gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);

                 write_string(" card no: ");
                 //BOOLEAN hat = TRUE;
                 for(INT8U i = 0; i < 8; i++){

                     key = get_keyboard();
                     if( key >= '0' && key <= '9')                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
                       {
                         gfprintf(COM2, "%c%c%c", 0x1B, 0xC4+i, key);
                         card_cif = key -'0';
                         write_int16u(card_cif);
                         xQueueSend(Q_CARD, &card_cif, 5);

                         if (i == 7){
                             write_string(" card entered ");
                             //write_int16u(get_queue_element(Q_CARD, 8, 7));
                             card_number_entered = TRUE;
                             order = 1;
                         }
                       } else {
                           i--;
                       }
                 }
                 break;

             case 1:

                 gfprintf(COM2, "%c%cPin code:    ", 0x1B, 0x80);
                 gfprintf(COM2, "%c%c                ", 0x1B, 0xA8);

                 write_string("pin: ");
                 //BOOLEAN hat = TRUE;
                 for(INT8U j = 0; j < 4; j++){

                     key = get_keyboard();
                     if( key >= '0' && key <= '9')                               // if it's a number between 0 and 9 we save that value in scale_tmp and go to the next state
                       {
                         gfprintf(COM2, "%c%c%c", 0x1B, 0xC6+j, key);
                         pin_cif = key - '0';
                         write_int16u(pin_cif);
                         xQueueSend(Q_PIN, &pin_cif, 0);
                         if (j == 3){
                             write_string(" pin entered ");
                             pin_code_entered = TRUE;
                             ui_state = 3;
                         }
                       } else {
                           j--;
                       }
                     }

                 break;

             }
            break;

            case 3:

                //Do if statement
//                if (!cardpin){
//                    //if (card_try == 0){
//                    ui_state = 1; // if the card number and pin doesn't match, go to 'choose payment type' again ----- or try three times and then go to ui_state 1
//                    //}else{
//                    //k--}
//                } else{

                paytype_complete = TRUE;
                write_string(" Complete ");
                break;
//                }
            }
        }
        vTaskDelay(  100 / portTICK_PERIOD_MS);
    }

}


INT8U wr_ch_LCD( INT8U Ch )
/*****************************************************************************
*   OBSERVE  : LCD_PROC NEEDS 20 mS TO PRINT OUT ONE CHARACTER
*   Function : See module specification (.h-file).
*****************************************************************************/
{


  return (xQueueSendToBack( Q_LCD, &Ch, portMAX_DELAY));
}

void wr_str_LCD( INT8U *pStr )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{

  while( *pStr )
  {
    wr_ch_LCD( *pStr );
    pStr++;
  }
}

void move_LCD( INT8U x, INT8U y )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  INT8U Pos;

  Pos = y*0x40 + x;
  Pos |= 0x80;
  wr_ch_LCD( ESC );
  wr_ch_LCD( Pos );
}
//----------------------------

void wr_ctrl_LCD_low( INT8U Ch )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Write low part of control data to LCD.
******************************************************************************/
{
  INT8U temp;
  volatile int i;

  temp = GPIO_PORTC_DATA_R & 0x0F;
  temp  = temp | ((Ch & 0x0F) << 4);
  GPIO_PORTC_DATA_R  = temp;
  for( i=0; i<1000; i )
      i++;
  GPIO_PORTD_DATA_R &= 0xFB;        // Select Control mode, write
  for( i=0; i<1000; i )
      i++;
  GPIO_PORTD_DATA_R |= 0x08;        // Set E High

  for( i=0; i<1000; i )
      i++;

  GPIO_PORTD_DATA_R &= 0xF7;        // Set E Low

  for( i=0; i<1000; i )
      i++;
}

void wr_ctrl_LCD_high( INT8U Ch )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Write high part of control data to LCD.
******************************************************************************/
{
  wr_ctrl_LCD_low(( Ch & 0xF0 ) >> 4 );
}

void out_LCD_low( INT8U Ch )
/*****************************************************************************
*   Input    : Mask
*   Output   : -
*   Function : Send low part of character to LCD.
*              This function works only in 4 bit data mode.
******************************************************************************/
{
  INT8U temp;

  temp = GPIO_PORTC_DATA_R & 0x0F;
  GPIO_PORTC_DATA_R  = temp | ((Ch & 0x0F) << 4);
  //GPIO_PORTD_DATA_R &= 0x7F;        // Select write
  GPIO_PORTD_DATA_R |= 0x04;        // Select data mode
  GPIO_PORTD_DATA_R |= 0x08;        // Set E High
  GPIO_PORTD_DATA_R &= 0xF7;        // Set E Low
}

void out_LCD_high( INT8U Ch )
/*****************************************************************************
*   Input    : Mask
*   Output   : -
*   Function : Send high part of character to LCD.
*              This function works only in 4 bit data mode.
******************************************************************************/
{
  out_LCD_low((Ch & 0xF0) >> 4);
}

void wr_ctrl_LCD( INT8U Ch )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Write control data to LCD.
******************************************************************************/
{
  static INT8U Mode4bit = 0;
  INT16U i;

  wr_ctrl_LCD_high( Ch );
  if( Mode4bit )
  {
    for(i=0; i<1000; i++);
    wr_ctrl_LCD_low( Ch );
  }
  else
  {
    if( (Ch & 0x30) == 0x20 )
      Mode4bit = 1;
  }
}

void clr_LCD()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Clear LCD.
******************************************************************************/
{
  gfprintf(COM2, "%c%c                                                                                       ", 0x1B, 0x80);
  //wr_ctrl_LCD( 0x01 );
}


void home_LCD()
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Return cursor to the home position.
******************************************************************************/
{
  wr_ctrl_LCD( 0x02 );
}

void Set_cursor( INT8U Ch )
/*****************************************************************************
*   Input    : New Cursor position
*   Output   : -
*   Function : Place cursor at given position.
******************************************************************************/
{
  wr_ctrl_LCD( Ch );
}


void out_LCD( INT8U Ch )
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Write control data to LCD.
******************************************************************************/
{
  INT16U i;

  out_LCD_high( Ch );
  for(i=0; i<1000; i++);
  out_LCD_low( Ch );
}





void lcd_task(void* pvParameters)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  INT8U ch;
  INT8U lcd_state = LCD_POWER_UP;

  while (1)
  {

  switch( lcd_state )
  {
    case LCD_POWER_UP:
      LCD_init = 0;
      lcd_state = LCD_INIT ;

      vTaskDelay(  50 / portTICK_PERIOD_MS);
      break;

    case LCD_INIT:
      if( LCD_init_sequense[LCD_init] != 0xFF )
        wr_ctrl_LCD( LCD_init_sequense[LCD_init++] );
      else
      {
        lcd_state = LCD_READY ;
      }
      vTaskDelay(  50 / portTICK_PERIOD_MS);
      break;

    case LCD_READY:
      if( xQueueReceive( Q_LCD, &ch, portMAX_DELAY ))
      {
        switch( ch )
        {
          case 0xFF:
            clr_LCD();
            break;
          case ESC:
            lcd_state = LCD_ESC_RECEIVED ;
            break;
          default:
            out_LCD( ch );
            vTaskDelay(  5 / portTICK_PERIOD_MS);
        }
      }
      break;

    case LCD_ESC_RECEIVED:
      if( xQueueReceive( Q_LCD, &ch, portMAX_DELAY ))
      {
        if( ch & 0x80 )
        {
            Set_cursor( ch );
        }
        else
        {
          switch( ch )
          {
            case '@':
                home_LCD();
              break;
          }
        }
        lcd_state = LCD_READY ;
        vTaskDelay(  5 / portTICK_PERIOD_MS);
      }
      break;
  }
  }
}


/****************************** End Of Module *******************************/




