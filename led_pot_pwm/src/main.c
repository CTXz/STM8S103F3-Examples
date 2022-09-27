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
#include <global_constants.h>

// Built-in LED (Pin B5, Active Low)
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_4

// Potentiometer
#define POT_GPIO_PORT GPIOD
#define POT_GPIO_PIN  GPIO_PIN_3
#define POT_ADC_CHANNEL ADC1_CHANNEL_4 // Channel 4 is connected to GPIO PD3 (See STM8CubeMX pinout for STM8S103F3Px)
#define POT_ADC_ADC_SCHMITTTRIG_CHANNEL ADC1_SCHMITTTRIG_CHANNEL4

#if F_CPU != 2000000UL
#error F_CPU set to wrong value! This example runs on 2MHz!
#error Please set the board_build.f_cpu option the platformio.ini file to 2000000UL!
#endif

// Main routine
void main(void)
{
	// Initialize GPIOs
	GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);	// LED: Output with push-pull, high level and 10MHz
	GPIO_Init(POT_GPIO_PORT, POT_GPIO_PIN, GPIO_MODE_IN_FL_NO_IT);	// Potentiometer: Input, Floating, No Ext Interrupt

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

	ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE); 	// Enable ADC interrupts
	ADC1_Cmd(ENABLE);			// Enable ADC
	
	// Initialze Timer 2
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, TIMER_PERIOD); 				  // 2Mhz/3 = cca. 666kHz
	TIM2_Cmd(ENABLE); 								  // Enable timer
	TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0, TIM2_OCPOLARITY_HIGH); // Initialize PWM output

	enableInterrupts(); // Enable interrupts

	while(TRUE); // Infinite loop, interrupts handle everything
}

// See: https://community.st.com/s/question/0D50X00009XkhigSAB/what-is-the-purpose-of-define-usefullassert
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (TRUE);
}
#endif
