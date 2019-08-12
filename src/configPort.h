#include "zeltapi.h"
pros::Motor frontLeft(1);
pros::Motor rearLeft(3);
pros::Motor frontRight(7,true);
pros::Motor rearRight(9,true);
pros::Motor leftLift(5);
pros::Motor rightLift(8,true);
pros::Motor leftRoller(6);
pros::Motor rightRoller(10,true);
pros::Motor leftClaw(19);
pros::Motor rightClaw(20);

int gyroPort = 1;
pros::ADIGyro gyro(gyroPort);
pros::Vision vision_sensor (20);

create_motor_group(fullDrive, frontLeft,rearLeft,frontRight,rearRight)
create_motor_group(leftDrive, frontLeft)
create_motor_group(rightDrive, frontRight, rearRight)
create_motor_group(liftgrp, leftLift)
create_motor_group(rollergrp, leftRoller,rightRoller)
create_motor_group(clawgrp, leftClaw)
