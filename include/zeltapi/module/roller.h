/**
 * \file zeltapi/module/roller.hpp
 *
 * roller header
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
#ifndef _ZELTAPI_ROLLER_H_
#define _ZELTAPI_ROLLER_H_
#include "core.hpp"
void stopRoller();
void holdRoller();
void releaseRoller();
void keepRoller(int speed,int waitms = 0);
Action moveRoller(float revolutions, int speed);
void initRoller();
#endif
