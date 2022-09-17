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
 * Description: Main file for the toggle_led_interrupt example
 * 		The main function initializes the GPIO pins and interrupts
 * 		and then simply enters an infinite loop awaiting interrupts.
 * 		The interrupt handler is defined in the EXTI_PORTD_IRQHandler 
 * 		in the stm8s_it.c file, which toggles the LED.
 */

// PlatformIO
#include <stm8s.h>

// include/
#include <stm8s_it.h>
#include <pins.h>

// Main routine
void main(void)
{
	GPIO_Init(LED_BUILTIN_PORT, LED_BUILTIN_PIN, GPIO_MODE_OUT_PP_LOW_FAST); // Built-in LED: Output, Push Pull, Low level, 10MHz
	GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_PU_IT);			 // Push button: Pull-up, Interrupt enabled

	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_ONLY);	 // Set interrupt sensitivity of PORTD to rising edge (button released)
	enableInterrupts(); 							 // Enable interrupts

	while(TRUE);
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
