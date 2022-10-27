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
 */

#include <midi.h>
#include <midi_msg.h>

void (*_note_on_callback)(uint8_t ch, midi_note_t note, uint8_t vel) = NULL;	///< Callback function when a Note On message is received
void (*_note_off_callback)(uint8_t ch, midi_note_t note, uint8_t vel) = NULL;	///< Callback function when a Note Off message is received

// Refer to header file for documentation
void on_note_on(void (*callback)(uint8_t ch, midi_note_t note, uint8_t vel))
{
	_note_on_callback = callback;
}

// Refer to header file for documentation
void on_note_off(void (*callback)(uint8_t ch, midi_note_t note, uint8_t vel))
{
	_note_off_callback = callback;
}