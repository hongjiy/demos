/*
 * ping.c
 *
 * 
 *  Author: Hongji Yang, HONGHAO LIU
 *	Documented by HONGHAO LIU on 4/28/15
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "util.h"
#include "lcd.h"
#include "open_interface.h"
#include "sensors.h"

volatile unsigned char state = 0;
volatile unsigned int last_time = 0;
volatile unsigned int current_time = 0;
volatile unsigned char update_flag = 0;
volatile unsigned int time_diff = 0;
volatile unsigned int overflow = 0;


/// Initializes PING))) sensor
/**
 * Initializes the PING))) sensor with noise canceler
 */
void init_sonar_cap()
{
	TCCR1A &= 0b11111100; //WGM11 = 0, WGM10 = 0
	TCCR1B |= 0b11000010; // Enable noise canceler, Edge select on rising edge, clk/8 prescaler
	TCCR1B &= 0b11100010; // Set WGM13 and WGM12 to 0, set bits 2 and 0 to 0
	TCCR1C &= 0b00000000; // Set FOC bits to 0
	TIMSK |= 0b00100100; // Enable input capture interrupt, enable overflow interrupt
}

/// Sends a pulse
/**
 * Sends a pulse from the PING))) sensor
 */
void send_pulse()
{
	TIMSK &= 0b11011111; // Disable IC interrupts
	DDRD |= 0x10; // Set pin 4 of port D to output
	PORTD |= 0x10; // Set pin 4 to high
	wait_ms(5); // wait the appropriate time
	
	PORTD &= 0xEF; // Set pin 4 to low
	DDRD &= 0xEF; // Set pin 4 as input
	
	TIFR |= 0b00100000; // Clear interrupt flag
	TIMSK |= 0b00100000; // Enable IC Interrupts
	state = 1; // State set to low
}

/// Converts the time from the PING))) sensor to distance in cm
/**
 * Converts the time given from the PING))) into distance in centimeters using the speed of sound.
 * @param time given from PING))) sensor
 * @return distance converted from time
 */
unsigned int time2dist(unsigned int time)
{
	// add formula
	return (34000 * time)/2000000; // Multiply time by speed of sound (in cm) according to Distance = Velocity * Time
}

/// Reads the time difference from the PING))) sensor
/**
 * Sends a pulse from the PING))) sensor and returns the distance in centimeters
 * @return time the signal edge is high converted to centimeters
 */
unsigned int ping_read()
{
	send_pulse();
	if(update_flag == 1)
	{ // Detect if there was an update
		time_diff = current_time - last_time; // Calculate width
		//lprintf("%u", time_diff);
		
		update_flag = 0;
	}
	
	return time2dist(time_diff); // Return time converted to distance
}
/// Interrupt that fires on an edge event
ISR (TIMER1_CAPT_vect)
{
	if(state == 1)
	{
		last_time = ICR1;
		TCCR1B &= 0b10111111; // Next firing happens on falling edge
		state = 2; // State set to high
	}
	else if(state == 2)
	{
		current_time = ICR1;
		TCCR1B |= 0b01000000; // Next firing happens on rising edge
		state = 0; // State set to done
	}
	else
	{
		// do nothing
	}
	update_flag = 1;
}

/// Interrupt that is triggered by overflow
ISR (TIMER1_OVF_vect)
{
	overflow++;
}