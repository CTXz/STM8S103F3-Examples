/*
 * Copyright (C) 2022 Patrick Pedersen

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * Description: Displays the button state on the built in LED
 * 		If the button is pressed, the LED will turn on
 * 		If the button is released, the LED will turn off
 * 
 * Pin Out:	BTN + : PD3
 *              BTN - : GND 
 */

#include <stm8s.h>

// Button
#define BTN_PORT 	 GPIOD
#define BTN_PIN  	 GPIO_PIN_3

// Built-in LED (Pin B5, Active Low)
#define LED_BUILTIN_PORT GPIOB
#define LED_BUILTIN_PIN  GPIO_PIN_5

// Main routine
void main(void)
{	
	// Initialize GPIOs
	GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_PU_NO_IT);			 // Button: Input with pull-up, no interrupts
	GPIO_Init(LED_BUILTIN_PORT, LED_BUILTIN_PIN, GPIO_MODE_OUT_PP_LOW_FAST); // Built-in LED: Output, Push Pull, Low level, 10MHz

	while(TRUE) {
		// Button released
		if (GPIO_ReadInputPin(BTN_PORT, BTN_PIN)) {
			GPIO_WriteHigh(LED_BUILTIN_PORT, LED_BUILTIN_PIN); // Turn off LED
		}
		// Button pressed
		else {
			GPIO_WriteLow(LED_BUILTIN_PORT, LED_BUILTIN_PIN); // Turn on LED
		}
	}
}

// See: https://community.st.com/s/question/0D50X00009XkhigSAB/what-is-the-purpose-of-define-usefullassert
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (TRUE)
	{
	}
}
#endif
