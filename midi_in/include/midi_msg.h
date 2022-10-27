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

#ifndef _MIDI_MSG_H_INCLUDED
#define _MIDI_MSG_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint8_t status;
  uint8_t data1;
  uint8_t data2;
} midi_msg_t;

#define MIDI_DATA_MSB_MASK 0x80

#define MIDI_STAT_CMD_MSK 0xF0
#define MIDI_STAT_NOTE_ON 0x90
#define MIDI_STAT_NOTE_OFF 0x80

#define MIDI_STAT_CH_MSK 0x0F

/**
 * @brief Check if a MIDI status byte indicates a Note On message
 * @param status_byte MIDI status byte
 * @return true if the status byte indicates a Note On message
 */
inline bool is_note_on(uint8_t status_byte)
{
	return (status_byte & MIDI_STAT_CMD_MSK) == MIDI_STAT_NOTE_ON;
}

/**
 * @brief Check if a MIDI status byte indicates a Note Off message
 * @param status_byte MIDI status byte
 * @return true if the status byte indicates a Note Off message
 */
inline bool is_note_off(uint8_t status_byte)
{
	return (status_byte & MIDI_STAT_CMD_MSK) == MIDI_STAT_NOTE_OFF;
}

/**
 * @brief Check if a MIDI status byte indicates a Note On or OFF message
 * @param status_byte MIDI status byte
 * @return true if the status byte indicates a Note On or Off message
 */
inline bool is_note_on_or_off(uint8_t status_byte)
{
	return (status_byte & 0xE0) == 0x80;
}

/**
 * @brief Returns the channel of a MIDI message
 * @param status_byte MIDI status byte
 * @return MIDI channel
 */
inline uint8_t channel(uint8_t status_byte)
{
	return status_byte & MIDI_STAT_CH_MSK;
}

/**
 * @brief Returns if the byte is a MIDI status byte
 * 
 * This function can be used to determine if a byte is a status byte.
 * 
 * A status byte must always have the most significant bit set.
 * 
 * This function is also useful to prevent misaligned data if the 
 * MCU cannot receive data fast enough.
 * 
 * @param data byte of data
 * @return true if the data is a status byte
 */
inline bool is_status(uint8_t data) {
	return (data & MIDI_DATA_MSB_MASK);
}

/**
 * @brief Returns if the byte is a MIDI data byte
 * 
 * This function can be used to determine if a byte is a data byte.
 * 
 * A data byte must always have the most significant bit set.
 * 
 * This function is also useful to prevent misaligned data if the 
 * MCU cannot receive data fast enough.
 * 
 * @param data byte of data
 * @return true if the data is a data byte
 */
#define is_data(data) (!is_status(data))

#endif //_MIDI_MSG_H_INCLUDED