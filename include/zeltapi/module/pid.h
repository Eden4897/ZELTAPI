#ifndef _ZELTAPI_PID_H_
#define _ZELTAPI_PID_H_
#include "main.h"
#include <vector>
#include <initializer_list>
struct PID{
  // Kp -  proportional gain
  // Ki -  Integral gain
  // Kd -  derivative gain
  // max - maximum value of manipulated variable
  // min - minimum value of manipulated variable

  double max,min,Kp,Ki,Kd;

  double _derivative_buffer = 0;

  double _output = 0;

  double _previous_error = 0;

  double _total_error = 0;

  double _error_change = 0;

  double _intergral_buffer = 0;

  double _target = 0;

  double _error = 0;

  PID(double Imax, double Imin, double IKp, double IKi, double IKd);

  void setUp(double setPoint,\
    double integralBuffer, double derivativeBuffer);

  double calculate(double input);

  void reset();
};
#endif
