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
 * Description: Blinks the built-in LED using the blocking delay_ms function.
 * 
 */

// PlatformIO
#include <stm8s.h>

// include/
#include <delay_ms.h>

#if F_CPU != 2000000UL
#error F_CPU set to wrong value! This example runs on 2MHz!
#error Please set the f_cpu option the platformio.ini file to 2000000UL!
#endif

// Built-in LED (Pin B5, Active Low)
#define LED_BUILTIN_PORT GPIOB
#define LED_BUILTIN_PIN  GPIO_PIN_5

void main(void)
{
	GPIO_Init(LED_BUILTIN_PORT, LED_BUILTIN_PIN, GPIO_MODE_OUT_PP_LOW_FAST); // Built-in LED: Output, Push Pull, Low level, 10MHz

	while(TRUE)
	{
		GPIO_WriteReverse(LED_BUILTIN_PORT, LED_BUILTIN_PIN); // Toggle built-in LED
		delay_ms(1000);
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
