/**
 * \file zeltapi/module/lift.hpp
 *
 * lift header
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
#ifndef _ZELTAPI_LIFT_H_
#define _ZELTAPI_LIFT_H_

#include "core.hpp"
#include "pid.h"

void holdLift();
void releaseLift();
void stopLift();
void keepLift(int speed, int waitms = 0);
void initLift();
Action moveLift(int target, int speed);
#endif
