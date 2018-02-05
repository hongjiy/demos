/*
 * music.c
 *
 * Created: 4/25/2015 2:08:08 PM
 *  Author: Daniel Shauger, Thiem Nguyen
 *  Documented by Daniel Shauger on 4/28/15
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "open_interface.h"
#include "sensors.h"


/// Load songs into the robot memory.
/**
 * Creates the song note-by-note and loads it into the robot memory
 */
void load_songs()
{	
	// ****COMMENTS FROM DAN DOYLE****
	//loads songs into memory, played with oi_play_song(int i);
	// Notes: oi_load_song takes four arguments
	// arg1 - an integer from 0 to 15 identifying this song
	// arg2 - an integer that indicates the number of notes in the song (if greater than 16, it will consume the next song index's storage space)
	// arg3 - an array of integers representing the midi note values (example: 60 = C, 61 = C sharp)
	// arg4 - an array of integers representing the duration of each note (in 1/64ths of a second)
	// 
	// For a note sheet, see page 12 of the iRobot Create Open Interface datasheet
	// needs oi_t self; oi_init(&self); before working
	
	// Musiccccccccc
	unsigned char guilesThemeNumNotes  = 50;
	unsigned char  guilesThemeNotes[50]    = {63, 63, 62, 0, 62, 63, 62, 63, 63, 62, 0, 62, 63, 62, 63, 62 , 63, 0, 62, 65, 0, 65, 63, 62, 58, 63, 63, 62, 0, 62, 63, 62, 63, 63, 62, 0, 62, 63, 62, 63, 62 , 63, 0, 62, 65, 0, 65, 63, 62, 58}; //Middle C = 60
	unsigned char guilesThemeDuration[50] = {16, 8, 8, 8, 8, 64, 16, 16, 8, 8, 8, 8, 64, 16, 8, 16, 8, 8, 16, 8, 8, 8, 16, 16, 16, 16, 8, 8, 8, 8, 64, 16, 16, 8, 8, 8, 8, 64, 16, 8, 16, 8, 8, 16, 8, 8, 8, 16, 16, 16};
	oi_load_song(0,  guilesThemeNumNotes, guilesThemeNotes, guilesThemeDuration);
}
