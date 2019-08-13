/**
 * \file module/drive.cpp
 *
 * all drive commands and functions
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
/*********************
 *    INCLUDES
 ********************/
 #include "_header.h"
 #include <vector>
 #include <initializer_list>

/*********************
 *	    CONFIG
 ********************/

bool _hold_move = 0;
int moveLspeed = 0;
int moveRspeed = 0;
int moveLimit = 0;
int movekeepms = 0;
float moveTick = 0;
int moveFix = 0;
int moveBuffer = 0;
int fixedAngle;
int absAngle;
std::vector<int> move_speed_set;

create_action_ptr(drive);

enum angle_detection_sensor_e{
  Gyro, Encoder
}angle_detection_sensor_e_t = Gyro;


enum brake_mode_e {
	MOVE_RELEASE, MOVE_BRAKE, MOVE_HOLD
}brake_mode_e_t;

void stopMove();
void endMove();

/*********************
 *      TASKS
 ********************/
void KEEP_MOVE_SERVICE(void* param) {
	wait(movekeepms);
	stopMove();
}

void HOLD_MOVE(void* param) {
	while (true) {
		if (_hold_move) {
			float absLeft = leftDrive().position();
			float absRight = rightDrive().position();

			while (_hold_move)
			{
				leftDrive().move(absLeft - leftDrive().position());
				rightDrive().move(absLeft - rightDrive().position());
				wait(5);
			}
		}
		wait(5);
	}
}


void DRIVE_SERVICE(void* param) {
	float leftPos, rightPos;
  float speed;
  int slowSpeed = move_speed_set[0];
  int fastSpeed = move_speed_set[1];
  int speedDiff = fastSpeed - slowSpeed;
  float percentFinished;

	driveptr->timer.startTimer();
	while (1) {
		leftPos = fabs(leftDrive().position());
		rightPos = fabs(rightDrive().position());

    percentFinished = fabs(fullDrive().position()) / moveTick;
    speed = fastSpeed - (percentFinished * speedDiff);

    leftDrive().move(speed);
  	rightDrive().move(speed);
  	moveLspeed = speed;
  	moveRspeed = speed;
		if (leftPos >= moveTick || rightPos >= moveTick) {
			driveptr->timer.stopTimer();
			endMove();
			break;
		}
		//returns in cm not ticks for simplicity
		driveptr->_value = (fullDrive().position() / 900) * (WHEEL_DIAMETER * 3.14159);
		wait(5);
	}

	driveptr->_end = true;
}


void ABS_DRIVE_SERVICE(void* param) {
	float leftPos, rightPos;
	float constSpeed = moveLspeed;
	float differ, fix;
  int speed;
  int initGyroPos = fixedAngle;
  int slowSpeed = move_speed_set[0];
  int fastSpeed = move_speed_set[1];
  int speedDiff = fastSpeed - slowSpeed;
  float percentFinished;
	driveptr->timer.startTimer();
	while (1) {
		differ = rightDrive().position(0) - leftDrive().position(0);
		fix = differ * moveFix;

		leftPos = fabs(leftDrive().position());
		rightPos = fabs(rightDrive().position());

    percentFinished = fabs(fullDrive().position()) / moveTick;
    speed = fastSpeed - (percentFinished * speedDiff);

    leftDrive().move(speed);
    rightDrive().move(speed);
    moveLspeed = speed;
    moveRspeed = speed;
    constSpeed = moveLspeed;

		if (leftPos >= moveTick || rightPos >= moveTick) {
			driveptr->timer.stopTimer();
			endMove();
			break;
		}
		if (fabs(differ) >= moveBuffer) {
			moveLspeed = constSpeed + fix;
			moveRspeed = constSpeed - fix;

			leftDrive().move(moveLspeed);
			rightDrive().move(moveRspeed);
		}
		//returns in cm not ticks for simplicity
		driveptr->_value = (fullDrive().position() / 900) * (WHEEL_DIAMETER * 3.14159);
		wait(5);
	}

	driveptr->_end = true;
}


// void PID_DRIVE_SERVICE(void* param){
//   float leftPos, rightPos;
//   pidLeftDrive().start(moveTick);
//   pidRightDrive().start(moveTick);
//   driveptr->timer.startTimer();
//   while(1){
//     leftPos = fabs(leftDrive().position());
//     rightPos = fabs(rightDrive().position());
//     leftDrive().move(pidLeftDrive().get());
//     rightDrive().move(pidRightDrive().get());
//     lcd_print(0,"moveTick = %d",moveTick);
//     lcd_print(1,"leftPos = %d",leftPos);
//     lcd_print(2,"rightPos = %d",rightPos);
//     if(leftPos>moveTick||rightPos>moveTick){
//       driveptr->timer.stopTimer();
//       endMove();
//       break;
//     }
//     //returns in cm not ticks for simplicity
//     driveptr->_value = (leftDrive().position() / 900) * (WHEEL_DIAMETER * 3.14159);
//     wait(5);
//   }
//
//   driveptr->_end = true;
// }


void TURN_SERVICE(void* param) {
  	float leftPos, rightPos;
    int speed;
    int initGyroPos = fixedAngle;
    int slowSpeed = move_speed_set[0];
    int fastSpeed = move_speed_set[1];
    int speedDiff = fastSpeed - slowSpeed;
    float percentFinished;
    int nowAngleDifference;//basically means the angle the robot has turned

  	driveptr->timer.startTimer();
  	while (1) {
      if(angle_detection_sensor_e_t == Encoder){
        leftPos = fabs(leftDrive().position());
    		rightPos = fabs(rightDrive().position());

        percentFinished = leftPos / moveTick;
        speed = fastSpeed - (percentFinished * speedDiff);

        leftDrive().move(-speed);
      	rightDrive().move(speed);
      	moveLspeed = -speed;
      	moveRspeed = speed;

    		if (leftPos >= moveTick || rightPos >= moveTick) {
    			driveptr->timer.stopTimer();
    			endMove();
    			break;
    		}
    		//returns in deg not ticks for simplicity
    		driveptr->_value = leftDrive().position() / TICKS_PER_DEGREE;
      }else{
        percentFinished = abs(nowAngleDifference) / moveTick;
        speed = fastSpeed - (percentFinished * speedDiff);

        leftDrive().move(-speed);
      	rightDrive().move(speed);
      	moveLspeed = -speed;
      	moveRspeed = speed;
        nowAngleDifference = abs(fixedAngle - initGyroPos);
        if(nowAngleDifference >= moveTick){
          driveptr->timer.stopTimer();
    			endMove();
    			break;
        }
        driveptr->_value = nowAngleDifference;
      }
  		wait(5);
  	}
	driveptr->_end = true;
}
void TURN_AZIMUTH_SERVICE(void* param){
  int initGyroPos = absAngle;
  int initAbsAngleDiff = fabs(initGyroPos - moveTick);
  int absAngleDiff;
  int speed;
  int slowSpeed = move_speed_set[0];
  int fastSpeed = move_speed_set[1];
  int speedDiff = fastSpeed - slowSpeed;
  float percentFinished;

  driveptr->timer.startTimer();
  while (1) {
    absAngleDiff = fabs(absAngle - moveTick);

    percentFinished = absAngleDiff / initAbsAngleDiff;
    speed = fastSpeed - (percentFinished * speedDiff);

    leftDrive().move(-speed);
    rightDrive().move(speed);
    moveLspeed = -speed;
    moveRspeed = speed;

    if(absAngle <= initGyroPos){
      driveptr->timer.stopTimer();
      endMove();
      break;
    }
    driveptr->_value = absAngle;
  }
    wait(5);
driveptr->_end = true;
}
void CURVE_SERVICE(void* param){

}
void GYRO_SERVICE(void* param){
  // pros::ADIAnalogIn sensor(gyroPort);
  // sensor.calibrate();
  gyro.reset();
  int nowPos;
  while(1){
    nowPos = gyro.get_value()/10;
    fixedAngle = nowPos;
    absAngle = fixedAngle % 360;
    wait(5);
  }
}


/*********************
 *	CORE FUNCTIONS
 ********************/
void initMove() {
	pros::Task hold_service(HOLD_MOVE);
  pros::Task gyro_service(GYRO_SERVICE);
}


void stopMove() {
	_hold_move = 0;
	leftDrive().move(0);
	rightDrive().move(0);
	moveTick = 0;
}


void keepMove(int leftspeed, int rightspeed, int waitms = 0) {
	stopMove();
	if (waitms == 0) {
		leftDrive().move(leftspeed);
		rightDrive().move(rightspeed);
	}
	else {
    Action drive{0};
    driveptr = &drive;
    component_type_e_t = Drive;
		moveTick = 1;
		movekeepms = waitms;
		leftDrive().move(leftspeed);
		rightDrive().move(rightspeed);
		pros::Task keep_move_service(KEEP_MOVE_SERVICE);
	}
}


void brakeMove(void* param) {
  Action drive{0};
  driveptr = &drive;
  component_type_e_t = Drive;
	_hold_move = 0;
	leftDrive().move(leftDrive().speed / -3.5);
	rightDrive().move(rightDrive().speed / -3.5);
	wait(200);
	leftDrive().move(0);
	rightDrive().move(0);
  moveTick = 0;
}


void brakeMove() {
	pros::Task braketask(brakeMove);
}


void holdMove() {
	moveTick = 0;
	_hold_move = true;
}


void releaseMove() {
	_hold_move = false;
}


void endMove() {
	switch (brake_mode_e_t) {
	case MOVE_RELEASE:
		stopMove();
		break;

	case MOVE_BRAKE:
		brakeMove();
		break;

	case MOVE_HOLD:
		holdMove();
		break;
	}
}



/*********************
 *	USER FUNCTIONS
 ********************/
Action forward(int cm,
               std::initializer_list<int> speedSet,
               brake_mode_e brakeMode = MOVE_BRAKE){
  Action drive{0};

  stopMove();

  leftDrive().tare();
  rightDrive().tare();

  move_speed_set = std::vector(speedSet);

  moveTick = ((cm / (WHEEL_DIAMETER * 3.14159)) * 900);

  driveptr = &drive;

  brake_mode_e_t = brakeMode;

  component_type_e_t = Drive;

	pros::Task drive_service(DRIVE_SERVICE);

  return drive;
}


Action forward(int cm,
               int speed = 0,
               brake_mode_e brakeMode = MOVE_BRAKE){
  return forward(cm,{speed,speed},brakeMode);
}


Action backward(int cm,
                std::initializer_list<int> speedSet,
                brake_mode_e brakeMode = MOVE_BRAKE){
  std::vector<int> temp = std::vector(speedSet);

  temp[0] = -temp[0];
  temp[1] = -temp[1];

  return forward(cm,{temp[0],temp[1]},brakeMode);
}


Action backward(int cm,
                int speed = 0,
                brake_mode_e brakeMode = MOVE_BRAKE) {
	return backward(cm, {speed,speed}, brakeMode);
}


Action absForward(int cm,
                  std::initializer_list<int> speedSet,
                  brake_mode_e brakeMode = MOVE_BRAKE,
                  float fix = 0.01,
                  float buffer = 1) {
	Action drive{ 0 };

	stopMove();

	leftDrive().tare();
	rightDrive().tare();

  move_speed_set = std::vector(speedSet);

	moveTick = ((cm / (WHEEL_DIAMETER * 3.14159)) * 900);

	driveptr = &drive;

	component_type_e_t = Drive;

	brake_mode_e_t = brakeMode;

	moveBuffer = buffer;

	moveFix = fix;

	pros::Task drive_service(ABS_DRIVE_SERVICE);

	return drive;
}

Action absForward(int cm,
                  int speed,
                  brake_mode_e brakeMode = MOVE_BRAKE,
                  float fix = 0.01,
                  float buffer = 1) {
return absForward(cm,{speed,speed},brakeMode,fix,buffer);
}
Action absBackward(int cm,
                   std::initializer_list<int> speedSet,
                   brake_mode_e brakeMode = MOVE_BRAKE,
                   float fix = 0.01,
                   float buffer = 1) {
  std::vector<int> temp = std::vector(speedSet);

  temp[0] = -temp[0];
  temp[1] = -temp[1];
	return absForward(cm,{temp[0],temp[1]}, brakeMode, fix, buffer);
}
Action absBackward(int cm,
                   int speed,
                   brake_mode_e brakeMode = MOVE_BRAKE,
                   float fix = 0.01,
                   float buffer = 1) {
return absBackward(cm,{speed,speed},brakeMode,fix,buffer);
}

Action turnLeft(int degrees,
                std::initializer_list<int> speedSet,
                brake_mode_e brakeMode = MOVE_BRAKE) {
	Action turn{ 0 };

	stopMove();

	leftDrive().tare();
	rightDrive().tare();

  move_speed_set = std::vector(speedSet);

  if(angle_detection_sensor_e_t == Encoder){
    moveTick = degrees * TICKS_PER_DEGREE;
  }else{
    moveTick = degrees;
  }
	driveptr = &turn;

	component_type_e_t = Drive;

	brake_mode_e_t = brakeMode;

	pros::Task turn_service(TURN_SERVICE);

	return turn;
}
Action turnLeft(int degrees,
                int speed,
                brake_mode_e brakeMode = MOVE_BRAKE) {
  return turnLeft(degrees,{speed,speed},brakeMode);
}

Action turnRight(int degrees,
                 std::initializer_list<int> speedSet,
                 brake_mode_e brakeMode = MOVE_BRAKE) {
  std::vector<int> temp = std::vector(speedSet);

  temp[0] = -temp[0];
  temp[1] = -temp[1];
	return turnLeft(degrees, {temp[0],temp[1]}, brakeMode);
}
Action turnRight(int degrees,
                int speed,
                brake_mode_e brakeMode = MOVE_BRAKE) {
  return turnRight(degrees,{speed,speed},brakeMode);
}
Action turnLeftToAzimuth(int degrees,
                         std::initializer_list<int> speedSet,
                         brake_mode_e brakeMode = MOVE_BRAKE){
  Action turn{absAngle};

  if(angle_detection_sensor_e_t == Gyro){
    stopMove();

  	leftDrive().tare();
  	rightDrive().tare();

    moveTick = degrees;

    driveptr = &turn;

  	component_type_e_t = Drive;

  	brake_mode_e_t = brakeMode;

  	pros::Task turn_azimuth_service(TURN_AZIMUTH_SERVICE);
  }
  return turn;
}
Action turnLeftToAzimuth(int degrees,
                         int speed,
                         brake_mode_e brakeMode = MOVE_BRAKE){
  return turnLeftToAzimuth(degrees,{speed,speed},brakeMode);
}
Action turnRightToAzimuth(int degrees,
                          std::initializer_list<int> speedSet,
                          brake_mode_e brakeMode = MOVE_BRAKE){
  std::vector<int> temp = std::vector(speedSet);

  temp[0] = -temp[0];
  temp[1] = -temp[1];
  return turnLeftToAzimuth(degrees, {temp[0],temp[1]}, brakeMode);
}
Action turnRightToAzimuth(int degrees,
                          int speed,
                          brake_mode_e brakeMode = MOVE_BRAKE){
  return turnRightToAzimuth(degrees,{speed,speed},brakeMode);
}
