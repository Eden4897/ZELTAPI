#include "module/core.hpp"
#include "module/pid.h"
extern_motor_group(fullDrive);
extern_motor_group(leftDrive);
extern_motor_group(rightDrive);
extern_motor_group(liftgrp);
extern_motor_group(rollergrp);
extern_motor_group(clawgrp);

extern_pid(pidLeftDrive);
extern_pid(pidRightDrive);

extern int gyroPort;
extern pros::ADIGyro gyro;
extern pros::Vision vision_sensor;

extern float WHEEL_DIAMETER;
extern float DEFAULT_FIX;
extern float DEFAULT_BUFFER;
extern float TICKS_PER_DEGREE;

extern float LIFT_MAX_LIMIT;
extern float LIFT_MIN_LIMIT;

extern float CLAW_MAX_LIMIT;
extern float CLAW_MIN_LIMIT;

extern component_type_e component_type_e_t;

extern bool isBlueSide;
extern int autoSelect;
extern int noOfAutos;
