//
//  USART.c
//  Contains the functions for communication between rover and computer
//
//  Final Project
//
//  Created by Hongji Yang, HONGHAO LIU on 4/13/15.
//  Documented by Daniel Shauger on 4/28/15
//  Copyright (c) 2015 thom. All rights reserved.
//


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "util.h"
#include "sensors.h"

// Make connection using bluetooth (1) or serial (0)
#define BLUETOOTH 1

// Determine which baud rate to use for communication
#if BLUETOOTH
	#define MYUBRR 34 // Bluetooth (57600 Baud)
#else
	#define MYUBRR 51 // RS232 (38400 Baud)
#endif

/// Initializes the serial connection.
/**
 * Initializes the serial connection using a predefined baud rate
 */
void USART_Init() 
{
	/// Set baud rate
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char)MYUBRR;
	
	/// Enable double speed mode
	UCSR0A = 0b00000010;
	
	/// Enable receive complete interrupt, receiving and transmitting data
	UCSR0B = 0b10011000; // (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	
	/// Set frame format: 8data, 2stop bit
	UCSR0C = 0b00001110; // (1<<USBS0) | (3<<UCSZ0);
}

/// Transmits a single character from the robot.
/** 
 * Puts character into output data register, when register is clear
 * @param data The character to send
 */
void serial_put_char(char data) 
{
	/// Wait for data register to clear before send the data
	while ((UCSR0A & 0b00100000) == 0);
	
	/// Put data into transmit buffer; sends the data
	UDR0 = data;
}

/// Sends a string over the serial connection.
/**
 * Sends a string over the serial connection by calling the serial_put_char(char data) function
 * @param data[] the string to send
 */
void serial_put_string(char data[]) 
{
	
	// Initialize array counter
	int i = 0;
	
	/// Send c-string to transmit register one character at a time
	while(data[i]) {
		serial_put_char(data[i++]);
	}
}

/// Receives data transmitted to the robot (from Putty, in our case).
/**
 * Receives the data transmitted and returns the data that is received
 * @return the data received by the robot
 */
unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

/// Transmits data from the robot (to Putty).
/**
 * Takes the data entered and sends it to Putty
 * @param data the data to send from the robot
 */
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
	UCSR0A |= 0b01000000;
}

/// Transmits a string from the robot (to Putty).
/**
 * Sends a string over a serial connection using the USART_Transmit function to send one char at a time
 * @param *data, the string to send
 */
void USART_Transmit_String(char *data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	int p;
	for(p = 0; p < strlen(data); p++)
	USART_Transmit(data[p]);
	UCSR0A |= 0b01000000;
}