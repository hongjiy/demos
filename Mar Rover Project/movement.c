/**
 * movement.c
 * Contains functions needed to control the robot's motion
 *
 * Final Project
 *
 *  Author: Hongji Yang, HONGHAO LIU on 3/18/15.
 *	Documented by Daniel Shauger on 4/28/15.
 * Copyright (c) 2015 thom. All rights reserved.
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "open_interface.h"
#include "util.h"
#include "sensors.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Thresholds for white tape for each sensor
#define WHITE_LEFT_TAPE_THRESHOLD 1200
#define WHITE_FRONTLEFT_TAPE_THRESHOLD 700
#define WHITE_FRONTRIGHT_TAPE_THRESHOLD 900
#define WHITE_RIGHT_TAPE_THRESHOLD 600

// Thresholds for red tape for each sensor
#define RED_LEFT_TAPE_THRESHOLD 1900
#define RED_FRONTLEFT_TAPE_THRESHOLD 1000
#define RED_FRONTRIGHT_TAPE_THRESHOLD 1300
#define RED_RIGHT_TAPE_THRESHOLD 800


/// Turns the robot the specified number of degrees.
/**
 * Turns the robot a specified number of degrees.
 * @param degrees the amount of degrees to turn
 * @param *sensor_data the oi_t sensor struct keeping track of measurements
 */
void turn(int degrees, oi_t *sensor_data) 
{
	int angle = 0;
	
	/// Set the wheels moving in the appropriate direction depending on the angle being turned (Clockwise is positive)
	if(degrees >= 0) 
	{
		/// Set the right wheel moving backwards and left forwards to rotate clockwise
		oi_set_wheels(-80, 80); 
	} 
	
	else 
	{
		/// Set the left wheel moving backwards and right forwards to rotate counter-clockwise
		oi_set_wheels(80, -80); 
	}
	
	/// Track how far the robot has rotated
	while (angle < abs(degrees)) 
	{
		/// update angle rotated since last sensor poll
		oi_update(sensor_data);
		angle += abs(sensor_data->angle); 
	}
	
	/// Stop rotation when the robot has moved at least the specified number of degrees
	oi_set_wheels(0,0); 
	

}

/// Moves the robot forwards and backwards.
/**
 * Moves the robot a specified distance at a specified speed.
 * @param distance the distance entered to move
 * @param speed the speed at which to move at; pass a negative value to move backwards
 * @param *sensor_data the oi_t sensor struct keeping open interface sensor measurments
 * @return the total distance moved
 */

int move(int distance, int speed, oi_t *sensor_data) 
{
	int distance_moved = 0;
	int danger_flag = 0;
	
	/// Check if starting in dangerous position
	oi_update(sensor_data);
	danger_flag = danger_detection(speed, sensor_data);
	
	/// Back up slightly if starting in dangerous position
	if (danger_flag != 0) 
	{
		move(2, -0.75 * speed, sensor_data);//maybe faster?
		USART_Transmit_String("Danger!\r\nMoved back 2 cm\r\n");
		return 0;
	}
	
	/// If no danger detected, move forward until the robot moves the specified distance
	/// While moving the robot checks continuously for dangers.
	oi_set_wheels(speed, speed);
	while (distance_moved < distance * 10)
	 {
		/// Update sensor data
		oi_update(sensor_data);
		danger_flag = danger_detection(speed, sensor_data);
		
		/// Backup from danger, if one is found
		if (danger_flag) 
		{
			distance = 0;
			move(0, -250, sensor_data);
			///USART_Transmit_String("Moved back 10 cm\r\n");
			USART_Transmit_String("Stopped\r\n");
			break;
		}
		
		/// Update distance moved
		distance_moved += abs(sensor_data->distance);
	}
	
	/// Stop moving
	oi_set_wheels(0, 0);
	
	
	danger_flag = 0;
	return distance_moved;
}

/// Detects danger based on the open interface sensor data
/**
 * Danger detection function; returns different values for different types of danger detected
 * @param speed the parameter to determine if the robot is moving
 * @param *sensor_data the oi_t sensor struct taking measurements from the open interface sensors
 * @return integer representation of danger detected (0 -> No danger, -1 -> Cliff Detected on Left, 1 -> Cliff Detected on Right, -3 -> Bumper Detected on Left, 3 -> Bumper Detected on Right)
 */
int danger_detection(int speed, oi_t *sensor_data)
{
	/// Only checks if the robot is moving forwards - No sensors are useful while reversing, and you can only reverse over terrain already driven across I guess..
	if(speed > 0)
	{
		
		/// Cliff Detection
		if(sensor_data->cliff_frontleft)
		{
			oi_set_wheels(0, 0); /// Stop the robot
			USART_Transmit_String("Cliff, frontleft sensor\r\n");
			return -1;
		}
		
		if(sensor_data->cliff_frontright)
		{
			oi_set_wheels(0, 0); /// Stop the robot
			USART_Transmit_String("Cliff, frontright sensor\r\n");
			return -1;
		}
		
		if(sensor_data->cliff_left)
		{
			oi_set_wheels(0, 0); /// Stop the robot
			USART_Transmit_String("Cliff, left sensor\r\n");
			return -1;
		}
		if(sensor_data->cliff_right)
		{
			oi_set_wheels(0, 0); /// Stop the robot
			USART_Transmit_String("Cliff, right sensor\r\n");
			return 1;
		}
    
		/// Tape Detection
		if (sensor_data->cliff_frontleft_signal >= WHITE_FRONTLEFT_TAPE_THRESHOLD)
		{
			if(sensor_data->cliff_frontleft_signal >= RED_FRONTLEFT_TAPE_THRESHOLD)
			{
				USART_Transmit_String("RED Tape, frontleft sensor\r\n");
				return 0;
			}
			
			else
			{
				oi_set_wheels(0, 0); /// Stop the robot
				USART_Transmit_String("Tape, frontleft sensor\r\n");
				return -2;
			}
		}
		
		if(sensor_data->cliff_left_signal >= WHITE_LEFT_TAPE_THRESHOLD)
		{
			if(sensor_data->cliff_left_signal >= RED_LEFT_TAPE_THRESHOLD)
			{
				USART_Transmit_String("RED Tape, left sensor\r\n");
				return 0;
			}
			
			else
			{
				oi_set_wheels(0, 0); /// Stop the robot
				USART_Transmit_String("Tape, left sensor\r\n");
				return -2;
			}
		}
		
		if(sensor_data->cliff_frontright_signal >= WHITE_FRONTRIGHT_TAPE_THRESHOLD)
		{
			if(sensor_data->cliff_frontright_signal >= RED_FRONTRIGHT_TAPE_THRESHOLD)
			{
				USART_Transmit_String("RED Tape, frontright sensor\r\n");
				return 0;
			}
			
			else
			{
				oi_set_wheels(0, 0); /// Stop the robot
				USART_Transmit_String("Tape, frontright sensor\r\n");
				return 2;
			}
		}
		
		if(sensor_data->cliff_right_signal >= WHITE_RIGHT_TAPE_THRESHOLD)
		{
			if(sensor_data->cliff_right_signal >= RED_RIGHT_TAPE_THRESHOLD)
			{
				USART_Transmit_String("RED Tape, right sensor\r\n");
				return 0;
			}
			
			else
			{
				oi_set_wheels(0, 0); /// Stop the robot
				USART_Transmit_String("Tape, right sensor\r\n");
				return 2;
			}
		}
		
		/// Bumper Detection
		if(sensor_data->bumper_left && sensor_data->bumper_right)
		{
			oi_set_wheels(0, 0); /// Stop the robot
			USART_Transmit_String("Bumper, left and right\r\n");
			return -3;
		}
		if(sensor_data->bumper_left)
		{
			oi_set_wheels(0, 0); /// Stop the robot
			USART_Transmit_String("Bumper, left\r\n");
			return -3;
		}
		if(sensor_data->bumper_right)
		{
			oi_set_wheels(0, 0); /// Stop the robot
			USART_Transmit_String("Bumper, right\r\n");
			return 3;
		}
	}
	return 0;
}