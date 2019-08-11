#include "zeltapi.h"

create_pid(pidLeftDrive,127,-127,1.5,0.005,0);
create_pid(pidRightDrive,127,-127,10,0,0);

float WHEEL_DIAMETER = 10.14;
float DEFAULT_FIX = 0.01;
float DEFAULT_BUFFER = 1;
float TICKS_PER_DEGREE = 4.53;

float LIFT_MAX_LIMIT = 900;
float LIFT_MIN_LIMIT = 1;

float CLAW_MAX_LIMIT = 1;
float CLAW_MIN_LIMIT = 1;
