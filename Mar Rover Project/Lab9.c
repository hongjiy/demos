/**
 * Lab9.c
 * Using robot 6
 * Created: 3/27/2015 2:16:37 PM
 *  Author: Thiem Nguyen, Daniel Shauger
 *  Documented by Daniel Shauger on 4/28/15
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "util.h"
#include "lcd.h"
#include "sensors.h"
#include <stdio.h>
#include <stdlib.h>
#include "open_interface.h"

#define MYUBRR 34
/// Thresholds for white tape for each sensor
#define WHITE_LEFT_TAPE_THRESHOLD 1200
#define WHITE_FRONTLEFT_TAPE_THRESHOLD 700
#define WHITE_FRONTRIGHT_TAPE_THRESHOLD 900
#define WHITE_RIGHT_TAPE_THRESHOLD 600

/// Thresholds for red tape for each sensor
#define RED_LEFT_TAPE_THRESHOLD 1900
#define RED_FRONTLEFT_TAPE_THRESHOLD 1000
#define RED_FRONTRIGHT_TAPE_THRESHOLD 1300
#define RED_RIGHT_TAPE_THRESHOLD 800

/// Struct that is used to describe objects
/**
 * Struct with four parameters for each object: 
 * distance to object, the angular width of that object, the linear width of each object, and the polar location of the object in relation to the object
 */
typedef struct
{
	char obj_dist;
	char ang_width;
	char width;
	char degree;
}object_T;

volatile unsigned char recieve_flag = 0;

char interpret_command(oi_t *sensor_data);
void poll(oi_t *sensor_data);
void raw_data(char *degrees, int *irDist, int *sonarDist);

///Conversion for angular width to linear width
/**
 * Uses the tangent function to convert the angular width to the linear width
 * based on the distance the object is from the robot
 * @param degrees the angular width of the object
 * @param dist the distance to the object
 * @return the linear width of the object converted from angular width
 */
int angle_to_linear(char degrees, int dist)
{
	return (int) round(2 * dist * tan(0.5 * degrees * (M_PI/ 180)));
}

/// Scans objects in a 180 degree field in front of the robot
/**
 * Uses the sonar and IR sensors to find and plot all objects in front of the robot,
 * and places them into an object array that is passed into the function
 * @param *degrees the array containing degrees that the servo increments through by 2
 * @param *irDist the array containing the IR distances for one scan
 * @param *sonarDist the array containing the sonar distances for one scan
 * @param *objects the array containing all detected objects in front of the robot
 */
void objScan(char *degrees, int *irDist, int *sonarDist, object_T *objects)
{
	int sum = 0;
	/// Iterators
	int i = 0, angle = 0, j = 0, o = 0;
	int object_flag = 0;
	int first_angle, second_angle;
	
	///scans
	move_servo(angle);
	wait_ms(300);
	USART_Transmit_String("Scanning");
	
	/// Sweep the servo sensor 180 degrees
	for(i = 0; i < 91; i++)
	{
		USART_Transmit('.'); /// Print a dot to Putty to show that the robot is still scanning
		move_servo(angle);
		
		degrees[i] = angle;
		
		///IR scan
		for(j = 0; j < 10; j++)
		{
			wait_ms(10);
			sum += ADC_read();
		}
		
		irDist[i] = D_to_distance(sum / 10);
		sum = 0;
		
		/// Sonar Scan
		sonarDist[i] = ping_read()/2;
		
		
		/// Finds the first drop in IRdist
		if(irDist[i] <= 100 && object_flag == 0 && o < 10)
		{
			first_angle = angle;
			object_flag = 1;
		}
		
		///Finds the rise in IRdist
		else if(irDist[i] > 100 && object_flag == 1 && o < 10)
		{
			second_angle = angle - 2;
			///If the the rise is right after the first angle then it's just noise
			if(second_angle != first_angle)
			{
				objects[o].ang_width = second_angle - first_angle;
				objects[o].obj_dist = sonarDist[i-1];
				objects[o].width = angle_to_linear(second_angle-first_angle, sonarDist[i-1]);
				///objects[o].degree = first_angle;
				objects[o].degree = (first_angle+second_angle)/2;
				o++;
			}
			///resets flag
			object_flag = 0;
			
		}
		///increments angle by 2
		angle += 2;
	}
	USART_Transmit_String("Done!\r\n");
}

/// Main
int main(void)
{
	/// Sensor initializations
    lcd_init();
    ADC_init();
	init_servo();
	init_sonar_cap();
	USART_Init(MYUBRR);
	///char value[100];
	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);
	
	/// Data values		
	char degrees[91];
	int irDist[91];
	int sonarDist[91];
	object_T objects[10];
	char command_value = 0;
	move_servo(0);
	load_songs();
	
    while(1)
    {
		/**
		*5/1/2015 Thiem Nguyen: Used this code to determine values for the
		*cliff sensors to find our specified colored circle
		*The code is kept as reference when we need to recalibrate for a different color
		*/
		/**
		For calibration of cliff sensors
		wait_ms(250);
		oi_update(sensor_data);
		lprintf("%d\n%d\n%d\n%d\n", 
			sensor_data->cliff_left_signal,  sensor_data->cliff_frontleft_signal,  sensor_data->cliff_frontright_signal,  sensor_data->cliff_right_signal);
		*/
		int k = 0;
		
		///Clear our object array
		for(k = 0; k < 10; k++)
		{
			objects[k].width = 0;
		}
		
		wait_ms(5);
		
		
		if(recieve_flag == 1)
		{
			command_value = interpret_command(sensor_data);
		}
		recieve_flag = 0;
		
		/// If scan command is received, scan for objects
		if(command_value == 1)
		{
			objScan(degrees, irDist, sonarDist, objects);
			
			int obj_i = 0;
			/// iterate through objects and transmit them to Putty
			while(objects[obj_i].width != 0)
			{
				char obj_data[50];
				sprintf(obj_data, "Object %d distance: %d cm\r\n", obj_i + 1, objects[obj_i].obj_dist);
				USART_Transmit_String(obj_data);
				sprintf(obj_data, "Object %d angular location: %d degrees\r\n", obj_i + 1, objects[obj_i].degree);
				USART_Transmit_String(obj_data);
				sprintf(obj_data, "Object %d angular width: %d cm\r\n",  obj_i + 1,objects[obj_i].ang_width);
				USART_Transmit_String(obj_data);
				sprintf(obj_data, "Object %d linear width: %d cm\r\n\n",  obj_i + 1,objects[obj_i].width);
				USART_Transmit_String(obj_data);
				obj_i++;
			}
			if(objects[0].width == 0){
				USART_Transmit_String("No objects found");
			}
			command_value = 0;
		}
		if(command_value == 2){
			raw_data(degrees, irDist, sonarDist);
			command_value = 0;
		}
	}
}

/// ISR fired when data received over serial connection
/**
 * USART0_RX_vect AVR vector for USART0 receive complete interrupt
 */
ISR(USART0_RX_vect) 
{
	recieve_flag = 1;
}

/// Returns 1 if scan command is received, otherwise interprets commands sent through putty
/**
 * Interprets the command recieved through Putty and takes the appropriate action.
 * @param *sensor_data the oi_t sensor struct that is taking measurements
 * @return 1 if the scan command is recieved, 0 otherwise
 */
char interpret_command(oi_t *sensor_data)
{
	/// Read in the sent command
	int command = USART_Receive();
	char com[50];
	
	sprintf(com, "\r\nCommand \"%c\" \r\n", command);
	USART_Transmit_String(com);
	/// Determine which action to take
	switch(command) { /// All movements have a sensor sweep after the action is completed
		case 'k': /// kill any actions currently going on
			oi_set_wheels(0, 0);
			break;
		case 'w': /// Move forward 10 cm
			///USART_Transmit_String("I moved %d cm", move(10, 250, sensor_data));
			USART_Transmit_String("Moving 10 cm\r\n");
			move(10, 250, sensor_data);
			break;
		case '2':
			USART_Transmit_String("Moving 20 cm\r\n");
			move(20, 250, sensor_data);
			break;
		case '3':
			USART_Transmit_String("Moving 30 cm\r\n");
			move(30, 250, sensor_data);
			break;
		case '4':
			USART_Transmit_String("Moving 40 cm\r\n");
			move(40, 250, sensor_data);
			break;
		case '5':
			USART_Transmit_String("Moving 50 cm\r\n");
			move(50, 250, sensor_data);
			break;
		case '0':
			USART_Transmit_String("Moving 100 cm\r\n");
			move(100, 250, sensor_data);
			break;
		case 's': /// Move backward 10 cm
			move(10, -250, sensor_data);
			USART_Transmit_String("Moved -10 cm\r\n");
			break;
		case 'q': /// Rotate Left 5deg
			turn(-5, sensor_data);
			USART_Transmit_String("Turned left 5 degrees\r\n");
			break;
		case 'a': /// Rotate left 15deg
			turn(-13, sensor_data);
			USART_Transmit_String("Turned left 15 degrees\r\n");
			break;
		case 'Q': /// Rotate left 45deg
			turn(-41, sensor_data);
			USART_Transmit_String("Turned left 45 degrees\r\n");
			break;
		case 'A': /// Rotate left 90 deg
			turn(-81, sensor_data);
			USART_Transmit_String("Turned left 90 degrees\r\n");
			break;
		case 'e': /// Rotate right 5 deg
			turn(5, sensor_data);
			USART_Transmit_String("Turned right 5 degrees\r\n");
			break;
		case 'd': /// Rotate right 15 deg
			turn(13, sensor_data);
			USART_Transmit_String("Turned right 15 degrees\r\n");
			break;
		case 'E': /// Rotate right 45 deg
			turn(41, sensor_data);
			USART_Transmit_String("Turned right 45 degrees\r\n");
			break;
		case 'D': /// Rotate right 90 deg
			turn(82, sensor_data);
			USART_Transmit_String("turned right 90 degrees\r\n");
			break;
		case 'S': /// Rotate 180
			turn(165, sensor_data);
			USART_Transmit_String("Turned around\r\n");
			break;
		case 'l': /// Look around (sensor sweep)
			return 1;
			break;
		case 'p': /// Poll Sensors
			oi_update(sensor_data);
			///danger_detection(sensor_data);
			poll(sensor_data);
			USART_Transmit_String("\r\n");
			break;
		case 'm': /// Mission Complete
			oi_set_wheels(0, 0);
			oi_set_leds(1, 1, 255, 255);
			oi_play_song(0);
			USART_Transmit_String("\r\n MISSION COMPLETE");
			break;
		case 'r':/// sends raw data
			return 2;
		default:
			USART_Transmit_String("Incorrect command\r\n");
			break;
	}
	
	return 0;
}


/// Sends the data from the LED sensors
/*
 * Finds what all the sensors are currently reporting.
 * @param *sensor_data the oi_t sensor struct keeping track of sensor values
 */
void poll(oi_t *sensor_data){
	char obj_data[50];
	USART_Transmit_String("\r\nPolling Data:\r\n");
	/// Left sensor
	sprintf(obj_data, "\r\nLeft Sensor hole: %d signal value: %d\r\n", sensor_data->cliff_left, sensor_data->cliff_left_signal);
	USART_Transmit_String(obj_data);
	if(sensor_data->cliff_left_signal >= WHITE_LEFT_TAPE_THRESHOLD)
	{
		if(sensor_data->cliff_left_signal >= RED_LEFT_TAPE_THRESHOLD)
		{
			USART_Transmit_String("RED Tape\r\n");
		}
		
		else
		{
			USART_Transmit_String("Tape\r\n");
		}
	}
	/// Frontleft sensor
	sprintf(obj_data, "\r\nFront left Sensor hole: %d signal value: %d\r\n", sensor_data->cliff_frontleft, sensor_data->cliff_frontleft_signal);
	USART_Transmit_String(obj_data);
	if (sensor_data->cliff_frontleft_signal >= WHITE_FRONTLEFT_TAPE_THRESHOLD)
	{
		if(sensor_data->cliff_frontleft_signal >= RED_FRONTLEFT_TAPE_THRESHOLD)
		{
			USART_Transmit_String("RED Tape\r\n");
		}
		
		else
		{
			USART_Transmit_String("Tape\r\n");
		}
	}
	/// Frontright sensor
	sprintf(obj_data, "\r\nFront right Sensor hole: %d signal value: %d\r\n", sensor_data->cliff_frontright, sensor_data->cliff_frontright_signal);
	USART_Transmit_String(obj_data);
	if(sensor_data->cliff_frontright_signal >= WHITE_FRONTRIGHT_TAPE_THRESHOLD)
	{
		if(sensor_data->cliff_frontright_signal >= RED_FRONTRIGHT_TAPE_THRESHOLD)
		{
			USART_Transmit_String("RED Tape\r\n");
		}
		
		else
		{
			USART_Transmit_String("Tape\r\n");
		}
	}
	/// Right sensor
	sprintf(obj_data, "\r\nRight Sensor hole: %d signal value: %d\r\n", sensor_data->cliff_right, sensor_data->cliff_right_signal);
	USART_Transmit_String(obj_data);
	if(sensor_data->cliff_right_signal >= WHITE_RIGHT_TAPE_THRESHOLD)
	{
		if(sensor_data->cliff_right_signal >= RED_RIGHT_TAPE_THRESHOLD)
		{
			USART_Transmit_String("RED Tape\r\n");
		}
		
		else
		{
			USART_Transmit_String("Tape\r\n");
		}
	}
}

/// Returns the scanned data in raw form.
/**
 * Takes the data arrays scanned in from the IR and PING))) sensors and prints the raw data to Putty
 * @param *degrees 
 * @param *irDist
 * @param *sonarDist
 */
void raw_data(char *degrees, int *irDist, int *sonarDist){
	int i;
	char value[100];
	USART_Transmit_String("Sending Raw Data!!\r\n");
	for(i = 0; i < 91; i++)
	{
		sprintf(value, "%u\t\t%u\t\t\t%u\r\n", degrees[i], irDist[i], sonarDist[i]);
		USART_Transmit_String(value);
		wait_ms(5);
	}
}