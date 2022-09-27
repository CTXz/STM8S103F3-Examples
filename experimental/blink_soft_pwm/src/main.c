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

#define PERIOD 0xFFFF/3 // With F_CPU = 2MHz, and prescaler = 1, we get a soft PWM frequency of approx 91.5Hz
			// 2MHz/(FFFF/3) = 91.5Hz, 0xFFFF is the max period value that can be set in the timer
			//
			// We can get faster PWM frequencies the following ways:
			//	- Increase the master clock frequency (F_CPU)
			// 	- Use a smaller period, at the cost of resolution 
			//
			// For setting LED brightness, 91.5Hz is fast enough to be perceived as continuous

#define BRIGHT_2_DUTY_VAL(x) (PERIOD/255) * x // Calculates when to turn off the LED based on a 0-255 brightness value

#define IT_PER_MS 28

volatile uint16_t _c;

// Soft PWM function, call continously to get a soft PWM signal
inline void update_soft_pwm(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin, uint8_t brightness) {
	
	static uint8_t counter = 0;

	uint16_t duty_val = BRIGHT_2_DUTY_VAL(brightness);
	
	if (TIM2_GetCounter() < duty_val)
		GPIO_WriteLow(GPIOx, GPIO_Pin);
	else
		GPIO_WriteHigh(GPIOx, GPIO_Pin);
}

inline void soft_pwm_for(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin, uint8_t brightness, uint16_t ms) {
	uint32_t i = (uint32_t)ms * IT_PER_MS;
	while(--i) {
		update_soft_pwm(GPIOx, GPIO_Pin, brightness);
	}
}

// Main routine
void main(void) {
	GPIO_Init(LED_BUILTIN_PORT, LED_BUILTIN_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, PERIOD);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_Cmd(ENABLE);

	uint8_t brght = 0;
	
	while(TRUE) {
		for (int i = 0; i < 255; i++)
			soft_pwm_for(LED_BUILTIN_PORT, LED_BUILTIN_PIN, i, 1);
		for (int i = 255; i >= 0; i--)
			soft_pwm_for(LED_BUILTIN_PORT, LED_BUILTIN_PIN, i, 1);
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
