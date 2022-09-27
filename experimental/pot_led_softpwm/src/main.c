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

// Built-in LED (Pin B5, Active Low)
#define LED_BUILTIN_PORT GPIOB
#define LED_BUILTIN_PIN  GPIO_PIN_5

// Potentiometer
#define POT_GPIO_PORT GPIOD
#define POT_GPIO_PIN  GPIO_PIN_3
#define POT_ADC_CHANNEL ADC1_CHANNEL_4 // Channel 4 is connected to GPIO PD3 (See STM8CubeMX pinout for STM8S103F3Px)
#define POT_ADC_ADC_SCHMITTTRIG_CHANNEL ADC1_SCHMITTTRIG_CHANNEL4

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

#define BRIGHT_2_DUTY_VAL(x) (PERIOD/4095) * x // Calculates when to turn off the LED based on a 0-255 brightness value

// Soft PWM function, call continously to get a soft PWM signal
inline void update_soft_pwm(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin, uint16_t brightness) {
	uint16_t duty_val = BRIGHT_2_DUTY_VAL(brightness);

	if (TIM2_GetCounter() < duty_val)
		GPIO_WriteLow(GPIOx, GPIO_Pin);
	else
		GPIO_WriteHigh(GPIOx, GPIO_Pin);
}

// Main routine
void main(void) {
	GPIO_Init(LED_BUILTIN_PORT, LED_BUILTIN_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(POT_GPIO_PORT, POT_GPIO_PIN, GPIO_MODE_IN_FL_NO_IT);

	TIM2_TimeBaseInit(TIM2_PRESCALER_1, PERIOD);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_Cmd(ENABLE);

	// Initialize ADC1
	ADC1_Init(
		ADC1_CONVERSIONMODE_CONTINUOUS,  // Continuous conversion mode
		POT_ADC_CHANNEL,		 // Channel to convert
		ADC1_PRESSEL_FCPU_D2,		 // Prescaler: fCPU/2
		ADC1_EXTTRIG_GPIO,		 // External trigger: GPIO (Irrelevant, as we're disabling the trigger)
		DISABLE, 			 // Disable triggers
		ADC1_ALIGN_RIGHT,		 // ADC data alignment: Right (Lest significant 8 bits are in low register, most significant 2 bits in high register)
		POT_ADC_ADC_SCHMITTTRIG_CHANNEL, // Selects schmitt trigger for channel 4
		DISABLE				 // State to which selected schmitt trigger should be set to
						 // Disabling schmitt trigger is recommended by the STM8S reference manual (See section 11.7.3, Table 23)
	);
	
	ADC1_Cmd(ENABLE); // Enable ADC1

	uint16_t brightness;
	while(TRUE) {
		ADC1_StartConversion(); 				// Start conversion
		while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == !SET); 	// Wait for conversion to finish
		brightness = ADC1_GetConversionValue(); 		// Get conversion value
		ADC1_ClearFlag(ADC1_FLAG_EOC); 				// Clear EOC (End-Of-Conversion) flag

		update_soft_pwm(LED_BUILTIN_PORT, LED_BUILTIN_PIN, brightness);
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
