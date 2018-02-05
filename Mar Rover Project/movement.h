//
//  movement.h
//  Contains function definitions related to robot movement
//
//  Final Project
//
//  Author: Hongji Yang, HONGHAO LIU on 3/18/15.
//  Copyright (c) 2015 thom. All rights reserved.
//

#include "open_interface.h"
/// Function to turn the robot the specified number of degrees
void turn(int degrees, oi_t *sensor_data);

/// Used to move the robot forwards and backwards
int move(int distance, int speed, oi_t *sensor_data);

/// Hazard Detection function
int danger_detection(int speed, oi_t *sensor_data);