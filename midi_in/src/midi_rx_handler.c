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

#include <stm8s.h>

#include <midi.h>
#include <midi_msg.h>

#include <debug.h>

typedef enum {
	STATUS,
	DATA1,
	DATA2
} midi_rx_state_t;

/**
 * @brief Calls callback functions after a MIDI message has been received
 * @param msg Pointer to a @ref midi_msg_t struct
 */
inline void on_midi_rx_complete(midi_msg_t *msg)
{
	uint8_t ch = channel(msg->status);

	if (is_note_on(msg->status)) {
		dbg_print("on_midi_rx_complete: Note On received\n");

		if (_note_on_callback != NULL) {
			dbg_print("on_midi_rx_complete: Calling Note On callback\n");
			_note_on_callback(ch, msg->data1, msg->data2);
		} else {
			dbg_print("on_midi_rx_complete: No callback registered\n");
		}
	}
	else if (is_note_off(msg->status)) {
		dbg_print("on_midi_rx_complete: Note Off received\n");

		if (_note_off_callback != NULL) {
			dbg_print("on_midi_rx_complete: Calling Note Off callback\n");
			_note_off_callback(ch, msg->data1, msg->data2);
		} else {
			dbg_print("on_midi_rx_complete: No callback registered\n");
		}
	}
}

/**
 * @brief Filters out unused/unsupported MIDI messages
 * 
 * The function is used to "filter" out MIDI messages that 
 * are not supported. This is to spare the MCU from having
 * to process messages that are not used. 
 * 
 * @param status MIDI status byte
 * @return true if the message type is supported
 */
inline bool status_filter(uint8_t status) {
	return is_note_on_or_off(status);
	// Add more filters here once more message 
	// types are supported
}

// See header file for documentation
void handle_midi_rx(uint8_t data)
{
	static midi_msg_t msg;
	static midi_rx_state_t state = STATUS;

	switch (state) {
		case STATUS:
			if (!status_filter(data))
				return;

			msg.status = data;
			state = DATA1;
			break;
		case DATA1:
			if (!is_data(data)) {
				state = STATUS;
				return;
			}

			msg.data1 = data;
			state = DATA2;
			break;
		case DATA2:
			if (!is_data(data)) {
				state = STATUS;
				return;
			}

			msg.data2 = data;
			state = STATUS;
			on_midi_rx_complete(&msg);
			break;
	}
}