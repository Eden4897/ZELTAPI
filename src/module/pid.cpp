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
  PID::PID(double Imax, double Imin, double IKp, double IKi, double IKd){
    this->max = Imax;
    this->min = Imin;
    this->Kp = IKp;
    this->Ki = IKi;
    this->Kd = IKd;
  }
  double PID::calculate(double inputPos){
    this->_error = this->_target - inputPos;
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
    Kp * _error +\
    Ki * _total_error +\
    Kd * _error_change;

    _output = _output < min? min:_output;
    _output = _output > max? max:_output;

    return _output;
  }
  void PID::setUp(double setPoint,\
    double integralBuffer, double derivativeBuffer){
    this->_target = setPoint;
    this->_intergral_buffer = integralBuffer;
    this->_derivative_buffer = derivativeBuffer;
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
