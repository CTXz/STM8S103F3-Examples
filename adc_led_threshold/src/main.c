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
 * Description: Main file for the adc_led_threshold project.
 */

#include <stm8s.h>

// Built-in LED
#define LED_BUILTIN_PORT GPIOB
#define LED_BUILTIN_PIN  GPIO_PIN_5

// Potentiometer
#define POT_GPIO_PORT GPIOD
#define POT_GPIO_PIN  GPIO_PIN_3
#define POT_ADC_CHANNEL ADC1_CHANNEL_4 // Channel 4 is connected to GPIO PD3 (See STM8CubeMX pinout for STM8S103F3Px)
#define POT_ADC_ADC_SCHMITTTRIG_CHANNEL ADC1_SCHMITTTRIG_CHANNEL4

#define MAX_ADC_VAL 1023 // Max value of 10 Bit ADC

// Main routine
void main(void)
{
	// Initialize GPIOs
	GPIO_Init(LED_BUILTIN_PORT, LED_BUILTIN_PIN, GPIO_MODE_OUT_PP_LOW_FAST);  // Built-in LED: Output with push-pull, low level and 10MHz

	GPIO_Init(POT_GPIO_PORT, POT_GPIO_PIN, GPIO_MODE_IN_FL_NO_IT);		  // Potentiometer: Input with floating input and no interrupts
										  //                Floating input is recommended for ADC inputs by 
										  // 		    the STM8S reference manual (See section 11.7.3, Table 23)

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

	uint16_t adc_val = 0; // Stores ADC value
	while(TRUE)
	{
		ADC1_StartConversion(); 				// Start conversion
		while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == !SET); 	// Wait for conversion to finish
		adc_val = ADC1_GetConversionValue(); 			// Get conversion value
		ADC1_ClearFlag(ADC1_FLAG_EOC); 				// Clear EOC (End-Of-Conversion) flag
		
		if (adc_val > MAX_ADC_VAL/2)					// Pot is above half way
			GPIO_WriteLow(LED_BUILTIN_PORT, LED_BUILTIN_PIN);	// Turn LED on
		else								// Pot is below half way
			GPIO_WriteHigh(LED_BUILTIN_PORT, LED_BUILTIN_PIN);	// Turn LED off
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
