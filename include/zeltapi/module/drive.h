/**
 * \file zeltapi/module/drive.h
 *
 * drive header
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
#ifndef _ZELTAPI_DRIVE_H_
#define _ZELTAPI_DRIVE_H_

#include "core.hpp"
#include "pid.h"

enum brake_mode_e{
  MOVE_RELEASE,MOVE_BRAKE,MOVE_HOLD
};
void stopMove();
void keepMove(int leftspeed,int rightspeed);
void brakeMove();
void holdMove();
void releaseMove();
Action forward(int cm,
               std::initializer_list<int> speedSet,
               brake_mode_e brakeMode = MOVE_BRAKE);
Action forward(int cm,
               int speed = 0,
               brake_mode_e brakeMode = MOVE_BRAKE);
Action backward(int cm,
                std::initializer_list<int> speedSet,
                brake_mode_e brakeMode = MOVE_BRAKE);
Action backward(int cm,
                int speed = 0,
                brake_mode_e brakeMode = MOVE_BRAKE);
Action absForward(int cm,
                  std::initializer_list<int> speedSet,
                  brake_mode_e brakeMode = MOVE_BRAKE,
                  float fix = 0.01,
                  float buffer = 1);
Action absForward(int cm,
                  int speed,
                  brake_mode_e brakeMode = MOVE_BRAKE,
                  float fix = 0.01,
                  float buffer = 1);
Action absBackward(int cm,
                   std::initializer_list<int> speedSet,
                   brake_mode_e brakeMode = MOVE_BRAKE,
                   float fix = 0.01,
                   float buffer = 1) ;
Action absBackward(int cm,
                   int speed = 0,
                   brake_mode_e brakeMode = MOVE_BRAKE,
                   float fix = 0.01,
                   float buffer = 1);
Action turnLeft(int degrees,
                std::initializer_list<int> speedSet,
                brake_mode_e brakeMode = MOVE_BRAKE);

Action turnLeft(int degrees,
                int speed,
                brake_mode_e brakeMode = MOVE_BRAKE);
Action turnRight(int degrees,
                 std::initializer_list<int> speedSet,
                 brake_mode_e brakeMode = MOVE_BRAKE);
Action turnRight(int degrees,
                int speed,
                brake_mode_e brakeMode = MOVE_BRAKE);
Action turnLeftToAzimuth(int degrees,
                         std::initializer_list<int> speedSet,
                         brake_mode_e brakeMode = MOVE_BRAKE);
Action turnLeftToAzimuth(int degrees,
                        int speed,
                        brake_mode_e brakeMode = MOVE_BRAKE);
Action turnRightToAzimuth(int degrees,
                          std::initializer_list<int> speedSet,
                          brake_mode_e brakeMode = MOVE_BRAKE);
Action turnRightToAzimuth(int degrees,
                          int speed,
                          brake_mode_e brakeMode = MOVE_BRAKE);
void initMove();

#endif
