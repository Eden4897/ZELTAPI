/**
 * \file zeltapi/module/pid.hpp
 *
 * pid header
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
#ifndef _ZELTAPI_PID_H_
#define _ZELTAPI_PID_H_
#include "main.h"
#include <vector>
#include <initializer_list>
struct PID{
  // Kp  - proportional gain
  // Ki  - Integral gain
  // Kd  - derivative gain
  // max - maximum value of manipulated variable
  // min - minimum value of manipulated variable
  double max,min,Kp,Ki,Kd;

  //the max value which the program will use Ki
  double _intergral_buffer = 0;

  //the min value which the program will use kD
  double _derivative_buffer = 0;

  double _output = 0;

  double _previous_error = 0;

  double _total_error = 0;

  double _error_change = 0;


  double _error = 0;

  PID(double inKp, double inKi, double inKd, double inMax, double inMin);

  double calculate(double setPoint, double value);

  void reset();
};
#endif
