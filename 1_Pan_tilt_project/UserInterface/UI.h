/*
 * UI.h
 *
 *  Created on: 23. apr. 2020
 *      Author: Patrick
 */

#ifndef USERINTERFACE_UI_H_
#define USERINTERFACE_UI_H_


void UI_receipt();
void show_report();
INT8U get_char_from_uart();


void add_to_sales_lf92(FP32 price);
void add_to_sales_lf95(FP32 price);
void add_to_sales_diesel(FP32 price);
void add_to_sum_of_cash(FP32 cashPrice);
void add_to_sum_of_card(FP32 cardPrice);
void add_to_total_op_time(INT16U seconds);

void report_task(void* pvParameters);

#endif /* USERINTERFACE_UI_H_ */
