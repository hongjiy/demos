/**
 * IR.c
 * Using robot 6
 *  Author: Thiem Nguyen, Daniel Shauger
 *  Documented by Daniel Shauger on 4/28/15
 */ 

#include <avr/io.h>
#include <math.h>
#include "util.h"
#include "lcd.h"
#include "open_interface.h"
#include "IR.h"

/// Initializes the ADC
/**
 * Initializes the ADC with 2.56 reference voltage and right adjusted results, one-shot activated and prescaler of 128.
 */
void ADC_init()
{
	ADMUX |= 0b11000000; ///2.56V reference and right adjusted Results
	ADCSRA |= 0b10000111; ///ADC enabled, One-shot activated, Prescaler 128
}

/// Retrieves data from the ADC
/**
 * Reads from the ADC and converts the analog value into a quantized value.
 * @return the quantized value from the ADC2 register                                                              
 */
int ADC_read()
{
	ADMUX |= 0b00000010; /// Reads from ADC2
	ADCSRA |= 0b01000000; /// Starts Conversion
	
	while(ADCSRA & 0b01000000); ///Checks if ADSC (bit 6) is still converting. ADSC bit will be 0 when it's finished.
	return ADC;
}

/// Converts the ADC-converted value to linear distance
/**
 * Takes passed value and converts to centimeters.
 * @param data the quantized data value from the ADC conversion
 * @return the quantized value converted to centimeters
 */
int D_to_distance(double data)
{
	return (int) round(23839.47646 * pow(data, -1.128661487)); 
}