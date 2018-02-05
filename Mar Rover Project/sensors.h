/*
 * ping.c
 *
 * 
 *  Author: Hongji Yang, HONGHAO LIU
 *	Documented by HONGHAO LIU on 4/28/15
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>
#include "util.h"
#include "lcd.h"
#include "open_interface.h"

void init_servo();
char move_servo(int angle);
void init_sonar_cap();
void send_pulse();
unsigned int time2dist(unsigned int time);
unsigned int ping_read();
void ADC_init();
int ADC_read();
int D_to_distance(double data);
void USART_Init();
void USART_Transmit_String(char *data);
unsigned char USART_Receive(void);
void serial_put_char(char data);
void serial_put_string(char data[]);
void USART_Transmit( unsigned char data );
void turn(int degrees, oi_t *sensor_data);
int move(int distance, int speed, oi_t *sensor_data);
int danger_detection(int speed, oi_t *sensor_data);
void load_songs();