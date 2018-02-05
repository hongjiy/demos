/*
 * servo.c
 *
 * 
 *  Author: Hongji Yang, Thiem Nguyen
 *	Documented by Daniel Shauger on 4/28/15
 */

#define TOP 43000

#include <avr/io.h>
#include "util.h"
#include "lcd.h"
#include "open_interface.h"
#include "sensors.h"


/// Initialize the servo
/**
 * Initializes the servo with mode 15, TOP stored in OCR3A and a prescaler of 8
 */
void init_servo()
{
	TCCR3A = 0x23; // 0b00100011 Non-inverting mode, Fast PWM mode (TOP in OCR3A) Mode 15
	TCCR3B = 0x1A; // 0b00011010 Fast PWM mode (TOP in OCR3A), prescaler of 8
	OCR3A = TOP; // Set TOP value
	DDRE |= 0x10; // Set wire 4 on Pin E to output
}

// Turns servo based on the entered counter value;
// (~1122 for 0 degrees, ~2933 for 90 degrees, ~4697 for 180 degrees
// Returns rough estimate for how long to wait for servo to turn

/// Moves the servo a given angle and returns a time to wait for the servo to turn
/**
 * Sets OCR3B with a counter value based on the given angle for the servo 
 * to turn to and returns a wait time appropriate to wait for the next servo movement.
 * @param angle to turn the robot
 * @return time to wait until the next servo movement
 */
char move_servo(int angle)
{
	unsigned int counter_value = ((angle * 3575.0) / 180) + 1122;
	OCR3B = counter_value;
	char wait_time = counter_value / 1000; // Edit this in lab for better values
	return wait_time;
}