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
void report_task(void* pvParameters);

#endif /* USERINTERFACE_UI_H_ */
