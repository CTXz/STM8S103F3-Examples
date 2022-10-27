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

#ifndef _MIDI_NOTES_H_INCLUDED
#define _MIDI_NOTES_H_INCLUDED

#include <stdint.h>

#define NOTE_C0  	0
#define NOTE_CS0 	1
#define NOTE_D0  	2
#define NOTE_DS0 	3
#define NOTE_E0  	4
#define NOTE_F0  	5
#define NOTE_FS0 	6
#define NOTE_G0  	7
#define NOTE_GS0 	8
#define NOTE_A0  	9
#define NOTE_AS0 	10
#define NOTE_B0  	11
#define NOTE_C1  	12
#define NOTE_CS1 	13
#define NOTE_D1  	14
#define NOTE_DS1 	15
#define NOTE_E1  	16
#define NOTE_F1  	17
#define NOTE_FS1 	18
#define NOTE_G1  	19
#define NOTE_GS1 	20
#define NOTE_A1  	21
#define NOTE_AS1 	22
#define NOTE_B1  	23
#define NOTE_C2  	24
#define NOTE_CS2 	25
#define NOTE_D2  	26
#define NOTE_DS2 	27
#define NOTE_E2  	28
#define NOTE_F2  	29
#define NOTE_FS2 	30
#define NOTE_G2  	31
#define NOTE_GS2 	32
#define NOTE_A2  	33
#define NOTE_AS2 	34
#define NOTE_B2  	35
#define NOTE_C3  	36
#define NOTE_CS3 	37
#define NOTE_D3  	38
#define NOTE_DS3 	39
#define NOTE_E3  	40
#define NOTE_F3  	41
#define NOTE_FS3 	42
#define NOTE_G3  	43
#define NOTE_GS3 	44
#define NOTE_A3  	45
#define NOTE_AS3 	46
#define NOTE_B3  	47
#define NOTE_C4  	48
#define NOTE_CS4 	49
#define NOTE_D4  	50
#define NOTE_DS4 	51
#define NOTE_E4  	52
#define NOTE_F4  	53
#define NOTE_FS4 	54
#define NOTE_G4  	55
#define NOTE_GS4 	56
#define NOTE_A4  	57
#define NOTE_AS4 	58
#define NOTE_B4  	59
#define NOTE_C5  	60
#define NOTE_CS5 	61
#define NOTE_D5  	62
#define NOTE_DS5 	63
#define NOTE_E5  	64
#define NOTE_F5  	65
#define NOTE_FS5 	66
#define NOTE_G5  	67
#define NOTE_GS5 	68
#define NOTE_A5  	69
#define NOTE_AS5 	70
#define NOTE_B5  	71
#define NOTE_C6  	72
#define NOTE_CS6 	73
#define NOTE_D6  	74
#define NOTE_DS6 	75
#define NOTE_E6  	76
#define NOTE_F6  	77
#define NOTE_FS6 	78
#define NOTE_G6  	79
#define NOTE_GS6 	80
#define NOTE_A6  	81
#define NOTE_AS6 	82
#define NOTE_B6  	83
#define NOTE_C7  	84
#define NOTE_CS7 	85
#define NOTE_D7  	86
#define NOTE_DS7 	87
#define NOTE_E7  	88
#define NOTE_F7  	89
#define NOTE_FS7 	90
#define NOTE_G7  	91
#define NOTE_GS7 	92
#define NOTE_A7  	93
#define NOTE_AS7 	94
#define NOTE_B7  	95
#define NOTE_C8  	96
#define NOTE_CS8 	97
#define NOTE_D8  	98
#define NOTE_DS8 	99
#define NOTE_E8  	100
#define NOTE_F8  	101
#define NOTE_FS8 	102
#define NOTE_G8  	103
#define NOTE_GS8 	104
#define NOTE_A8  	105
#define NOTE_AS8 	106
#define NOTE_B8  	107
#define NOTE_C9  	108
#define NOTE_CS9 	109
#define NOTE_D9  	110
#define NOTE_DS9 	111
#define NOTE_E9  	112
#define NOTE_F9  	113
#define NOTE_FS9 	114
#define NOTE_G9  	115
#define NOTE_GS9 	116
#define NOTE_A9  	117
#define NOTE_AS9 	118
#define NOTE_B9  	119
#define NOTE_C10 	120
#define NOTE_CS10 	121
#define NOTE_D10 	122
#define NOTE_DS10 	123
#define NOTE_E10 	124
#define NOTE_F10 	125
#define NOTE_FS10 	126
#define NOTE_G10 	127

typedef uint8_t midi_note_t;

#endif // _MIDI_NOTES_H_INCLUDED