/*
 * keypad.h
 *
 *  Created on: Apr 13, 2023
 *      Author: lineknudsen
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

// Inputs
#define ROW0 GPIO_PIN_8 // PD8
#define ROW1 GPIO_PIN_9 // PD9
#define ROW2 GPIO_PIN_14 // PDE
#define ROW3 GPIO_PIN_15 // PDF
#define NUM_ROWS 4

// Outputs
#define COL0 GPIO_PIN_4 // PD4
#define COL1 GPIO_PIN_5 // PD5
#define COL2 GPIO_PIN_6 // PD6
#define COL3 GPIO_PIN_7 // PD7
#define NUM_COLS 4

// Outputs
#define LED0 GPIO_PIN_0 // PD0
#define LED1 GPIO_PIN_1 // PD1
#define LED2 GPIO_PIN_2 // PD2
#define LED3 GPIO_PIN_3 // PD3

// Debounce parameters
#define DEBOUNCE_ATTEMPTS 50
#define DEBOUNCE_COUNT 30
#define DEBOUNCE_INTERVAL 1000

int ReadKeypad(void);

#endif /* INC_KEYPAD_H_ */
