/*
 * IR.h
 * Using robot 6
 * Created: 3/27/2015 2:16:37 PM
 *  Author: Thiem Nguyen, Daniel Shauger
 */ 

//Initializes the ADC(IR) sensor
void ADC_init();
//Reads a quantization value from IR
int ADC_read();
//Converts quantization value into distance in cm
int D_to_distance(double data);