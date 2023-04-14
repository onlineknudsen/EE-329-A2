/*
 * keypad.c
 *
 *  Created on: Apr 13, 2023
 *      Author: lineknudsen
 */

#include "main.h"
#include "keypad.h"

// Keypad LUT
const int keypadLUT[NUM_ROWS][NUM_COLS] = {
		{1, 2, 3, 0xA},
		{4, 5, 6, 0xB},
		{7, 8, 9, 0xC},
		{0xE, 0, 0xF, 0xD}
};

int ReadKeypad(void)
{
	int key = -1;
	// Loop through all rows
	for(uint8_t col = 0; col < 4; col++)
	{
		// Loop through all columns
		GPIOD->BSRR = (0b1 << col) << 4 | (~(0b1 << col) << 4) << 16;
		for(uint8_t row = 0; row < 4; row++)
		{
			// due to non-consecutive ports, we have to calculate the pin position from the given row
			int rowPos;
			if(row > 1)
			{
				rowPos = row % 2 + 14;
			}
			else
			{
				rowPos = row % 2 + 8;
			}

			uint16_t input = GPIOD->IDR & (0b1 << rowPos);
			// if this specific row and this specific column is pressed
			if (input)
			{
				// debounce it

				// Check for 30 highs in a row
				int highCount = 0;
				for(int i = 0; i < DEBOUNCE_ATTEMPTS; i++)
				{
					// Delay
					for(int j = 0; j < DEBOUNCE_INTERVAL; j++);
					input = GPIOD->IDR & (0b1 << rowPos);

					if(input)
						highCount++;
					else
						highCount = 0;

					// If we have 30 highs in a row
					if(highCount >= DEBOUNCE_COUNT)
						break;
				}

				// Map to number from LUT
				key = keypadLUT[row][col];
				return key;
			}
		}
	}

	return key;
}
