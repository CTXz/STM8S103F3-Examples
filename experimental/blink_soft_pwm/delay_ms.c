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
 * Description: Implementation of delay_ms function
 * 
 */

#include <delay_ms.h>

// Number of instructions required for a single loop with a decrementing register value
// decw(1) & jrne(2) = 3 instructions
#define CYCLES_PER_LOOP 3

const uint16_t LOOPS_PER_MS = F_CPU/CYCLES_PER_LOOP/1000; // Number of loops required for a millisecond to pass
volatile uint16_t _ms;					  // Required to pass ms parameter to asm code

void delay_ms(uint16_t ms) {
	_ms = ms;
	__asm
		ldw y, __ms 		// Load ms counter into y register: 2 cycles
	0000$:
		ldw x, _LOOPS_PER_MS 	// Load loop counter into x register: 2 cycles
	0001$: 				
		decw x 			// Decrease loop counter: 1 cycle 
		jrne 0001$		// Check if 1ms passed: 2 cycles (except for x == 0 where it's just 1 cycle)
				
		decw y			// Decrease ms counter: 1 cycle
		jrne 0000$		// Check if provided time has passed: 2 cycles (except for y == 0 where it's just 1 cycle)
	__endasm;
}