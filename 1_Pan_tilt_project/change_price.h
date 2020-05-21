/*
 * change_price.h
 *
 *  Created on: 20. maj 2020
 *      Author: Rebec
 */

#ifndef CHANGE_PRICE_H_
#define CHANGE_PRICE_H_
#include "emp_type.h"

void wr_str_UART(char* txt);
void wr_char_UART(char character);
void change_price_task(void *pvParameters);

#endif /* CHANGE_PRICE_H_ */
