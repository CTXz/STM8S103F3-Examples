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
 * Description: Template main file for STM8S103F3P6 PlatformIO projects
 */

// PlatformIO
#include <stm8s.h>

// include/
#include <stm8s_it.h>

// Built-in LED (Pin B5, Active Low)
#define LED_BUILTIN_PORT GPIOB
#define LED_BUILTIN_PIN  GPIO_PIN_5

#if F_CPU != 2000000UL
#error F_CPU set to wrong value! This example runs on 2MHz!
#error Please set the board_build.f_cpu option the platformio.ini file to 2000000UL!
#endif

#define T2_PRESCALER 2048
#define T2_COUNTS_PER_SECOND F_CPU/T2_PRESCALER

// Main routine
void main(void)
{
	GPIO_Init(LED_BUILTIN_PORT, LED_BUILTIN_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	
	TIM2_TimeBaseInit(TIM2_PRESCALER_2048, T2_COUNTS_PER_SECOND);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_Cmd(ENABLE);

	enableInterrupts();

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
