
#ifndef _ZELTAPI_CLAW_H_
#define _ZELTAPI_CLAW_H_
#include "core.hpp"
#include "pid.h"
void stopClaw();
void holdClaw();
void releaseClaw();
void keepClaw(int speed, int waitms = 0);
Action openClaw(int speed);
Action closeClaw(int speed);
void initClaw();
#endif
