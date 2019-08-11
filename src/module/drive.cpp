/*********************
	  includes
 ********************/
 #include "_header.h"

 /*********************
		 set up
  ********************/
bool _hold_move = 0;
int moveLspeed = 0,moveRspeed = 0,moveLimit = 0,movekeepms = 0;
float moveTick = 0, moveFix = 0, moveBuffer = 0;

enum angle_detection_sensor_e{
  Gyro, Encoder
}angle_detection_sensor_e_t = Gyro;

create_action_ptr(drive);

enum brake_mode_e {
	MOVE_RELEASE, MOVE_BRAKE, MOVE_HOLD
}brake_mode_e_t;
void stopMove();
void endMove();



/*********************
		 tasks
 ********************/
void KEEP_MOVE_SERVICE(void* param) {
	wait(movekeepms);
	stopMove();
}
extern_motor_group(leftDrive);

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

	driveptr->timer.startTimer();
	while (1) {
		leftPos = fabs(leftDrive().position());
		rightPos = fabs(rightDrive().position());
		if (leftPos > moveTick || rightPos > moveTick) {
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
	int counter = 0;
	driveptr->timer.startTimer();
	while (1) {
		differ = rightDrive().position(0) - leftDrive().position(0);
		fix = differ * moveFix;

		leftPos = fabs(leftDrive().position());
		rightPos = fabs(rightDrive().position());
		lcd_print(0, "moveTick = %d", moveTick);
		lcd_print(1, "leftPos = %d", leftPos);
		lcd_print(2, "rightPos = %d", rightPos);
		if (leftPos > moveTick || rightPos > moveTick) {
			driveptr->timer.stopTimer();
			endMove();
			break;
		}
		if (fabs(differ) >= moveBuffer) {
			counter++;
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

	driveptr->timer.startTimer();
	while (1) {
		leftPos = fabs(leftDrive().position());
		rightPos = fabs(rightDrive().position());
		lcd_print(0, "moveTick = %d", moveTick);
		lcd_print(1, "leftPos = %d", leftPos);
		lcd_print(2, "rightPos = %d", rightPos);
		if (leftPos > moveTick || rightPos > moveTick) {
			driveptr->timer.stopTimer();
			endMove();
			break;
		}
		//returns in cm not ticks for simplicity
		driveptr->_value = leftDrive().position() / TICKS_PER_DEGREE;
		wait(5);
	}

	driveptr->_end = true;
}



/*********************
	basic functions
 ********************/
void initMove() {
	pros::Task holdservice(HOLD_MOVE, (void*)"PROS");
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
		moveTick = 1;
		movekeepms = waitms;
		leftDrive().move(leftspeed);
		rightDrive().move(rightspeed);
		pros::Task keep_move_service(KEEP_MOVE_SERVICE);
	}
}


void brakeMove(void* param) {
	_hold_move = 0;
	moveTick = 0;
	leftDrive().move(leftDrive().speed / -3.5);
	rightDrive().move(rightDrive().speed / -3.5);
	wait(200);
	leftDrive().move(0);
	rightDrive().move(0);
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
	  functions
 ********************/
Action forward(int cm, int speed = 0, brake_mode_e brakeMode = MOVE_BRAKE) {
	Action move{ 0 };

	stopMove();

	leftDrive().tare();
	rightDrive().tare();

	moveTick = ((cm / (WHEEL_DIAMETER * 3.14159)) * 900);

	driveptr = &move;

	brake_mode_e_t = brakeMode;

	component_type_e_t = Drive;
	//activate pid?

	if (speed != 0) {

		leftDrive().move(speed);
		rightDrive().move(speed);
		moveLspeed = speed;
		moveRspeed = speed;

		pros::Task drive_service(DRIVE_SERVICE);

	}
	else {
		// pros::Task pid_drive_service(PID_DRIVE_SERVICE);
	}
	return move;
}


Action backward(int cm, int speed = 0, brake_mode_e brakeMode = MOVE_BRAKE) {
	return forward(cm, -speed, brakeMode);
}


Action absForward(int cm, int speed = 0, brake_mode_e brakeMode = MOVE_BRAKE, float fix = 0.01, float buffer = 1) {
	Action move{ 0 };

	stopMove();

	leftDrive().tare();
	rightDrive().tare();

	moveTick = ((cm / (WHEEL_DIAMETER * 3.14159)) * 900);

	driveptr = &move;

	component_type_e_t = Drive;

	brake_mode_e_t = brakeMode;

	moveBuffer = buffer;

	moveFix = fix;

	//activate pid?

	if (speed != 0) {

		leftDrive().move(speed);
		rightDrive().move(speed);
		moveLspeed = speed;
		moveRspeed = speed;

		pros::Task drive_service(ABS_DRIVE_SERVICE);

	}
	else {
		// pros::Task pid_drive_service(PID_ABS_DRIVE_SERVICE);
	}
	return move;
}


Action absBackward(int cm, int speed = 0, brake_mode_e brakeMode = MOVE_BRAKE, float fix = 0.01, float buffer = 1) {
	return absForward(cm, -speed, brakeMode, fix, buffer);
}


Action turnLeft(int degrees, int speed = 0, brake_mode_e brakeMode = MOVE_BRAKE) {
	Action turn{ 0 };

	stopMove();

	leftDrive().tare();
	rightDrive().tare();

	leftDrive().move(-speed);
	rightDrive().move(speed);
	moveLspeed = -speed;
	moveRspeed = speed;

	moveTick = degrees * TICKS_PER_DEGREE;

	driveptr = &turn;

	component_type_e_t = Drive;

	brake_mode_e_t = brakeMode;

	pros::Task turn_service(TURN_SERVICE);

	return turn;
}


Action turnRight(int degrees, int speed = 0, brake_mode_e brakeMode = MOVE_BRAKE) {
	return turnLeft(degrees, -speed, brakeMode);
}
