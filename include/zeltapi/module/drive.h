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
Action forward(int cm, int speed = 0, brake_mode_e brakeMode = MOVE_HOLD);
Action backward(int cm, int speed = 0, brake_mode_e brakeMode = MOVE_HOLD);
Action absForward(int cm, int speed = 0, brake_mode_e brakeMode = MOVE_HOLD, float fix = 0.01, float buffer = 1);
Action absBackward(int cm, int speed = 0, brake_mode_e brakeMode = MOVE_HOLD, float fix = 0.01, float buffer = 1);
Action turnLeft(int degrees, int speed = 0, brake_mode_e brakeMode = MOVE_HOLD);
Action turnRight(int degrees, int speed = 0, brake_mode_e brakeMode = MOVE_HOLD);
void initMove();

#endif
