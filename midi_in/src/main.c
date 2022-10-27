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

// Standard C
#include <string.h>

// PlatformIO
#include <stm8s.h>

// include/
#include <stm8s_it.h>
#include <midi.h>

// BUILTIN LED
#define BUILTIN_LED_PORT GPIOB
#define BUILTIN_LED_PIN  GPIO_PIN_5

// UART
#define UART_PORT GPIOD
#define UART_TX_PIN GPIO_PIN_5
#define UART_RX_PIN GPIO_PIN_6
#define UART_BAUD 31250

/**
 * @brief Initializes the clock to 16 MHz
 */
void init_16mhz_clk()
{
	CLK_DeInit();
	CLK_HSICmd(ENABLE);
	CLK_HSECmd(DISABLE);
	CLK_LSICmd(DISABLE);
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}

/**
 * @brief Callback function when a Note On message is received
 * 
 * @param ch MIDI channel
 * @param note MIDI note
 * @param vel MIDI velocity
 */
void note_on(uint8_t ch, midi_note_t note, uint8_t vel)
{
	GPIO_WriteLow(BUILTIN_LED_PORT, BUILTIN_LED_PIN);
}

/**
 * @brief Callback function when a Note Off message is received
 * 
 * @param ch MIDI channel
 * @param note MIDI note
 * @param vel MIDI velocity
 */
void note_off(uint8_t ch, midi_note_t note, uint8_t vel)
{
	GPIO_WriteHigh(BUILTIN_LED_PORT, BUILTIN_LED_PIN);
}

// Main routine
void main(void)
{
	init_16mhz_clk();

	// Initialize GPIOs
	GPIO_Init(BUILTIN_LED_PORT, BUILTIN_LED_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(UART_PORT, UART_TX_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(UART_PORT, UART_RX_PIN, GPIO_MODE_IN_PU_NO_IT);

	// Set up MIDI callbacks
	on_note_on(&note_on);
	on_note_off(&note_off);

	// Initialize UART1 peripheral
	UART1_Init
	(
		UART_BAUD,			// Baudrate: 9600
		UART1_WORDLENGTH_8D,		// Word length: 8 bits
		UART1_STOPBITS_1,		// Stop bits: 1
		UART1_PARITY_NO,		// Parity: None
		UART1_SYNCMODE_CLOCK_DISABLE,	// Sync mode: Clock disabled (Asynchronous)
		UART1_MODE_TXRX_ENABLE		// Mode: TX and RX enabled
	);

	UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);	// Enable RX interrupt	
	enableInterrupts();				// Enable interrupts

	UART1_Cmd(ENABLE); // Enable UART1 peripheral

	// Rest is handled by interrupts
	// See stm8s_it.c, line 347: INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
	while(TRUE)
	{
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
