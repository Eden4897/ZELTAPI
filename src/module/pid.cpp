/**
 * \file module/pid.cpp
 *
 * all functions is the PID class
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
#include "_header.h"
// Kp -  proportional gain
// Ki -  Integral gain
// Kd -  derivative gain
// max - maximum value of manipulated variable
// min - minimum value of manipulated variable
PID::PID(double inKp, double inKi, double inKd, double inMax, double inMin){
  this->Kp = inKp;
  this->Ki = inKi;
  this->Kd = inKd;
  this->max = inMax;
  this->min = inMin;
}

double PID::calculate(double setPoint, double value){
  this->_error = setPoint - inputPos;
  this->_error_change = _error - _previous_error;
  if (fabs(_error) < _intergral_buffer){
    this->_total_error += _error;
  }else{
    this->_total_error = 0;
  }
  if(fabs(_error) > _derivative_buffer){
    this->_error_change = 0;
  }
  this->_output = \
  Kp * _error +\       /*proportional*/
  Ki * _total_error +\ /*integral*/
  Kd * _error_change;  /*derivative*/

  _output = _output < min? min:_output;
  _output = _output > max? max:_output;

  return _output;
}

void PID::reset(){
  this->_derivative_buffer = 0;

  this->_output = 0;

  this->_previous_error = 0;

  this->_total_error = 0;

  this->_error_change = 0;

  this->_intergral_buffer = 0;

  this->_target = 0;

  this->_error = 0;
}
