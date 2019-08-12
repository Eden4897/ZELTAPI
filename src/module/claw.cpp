#include "_header.h"

bool _hold_claw = 0;
int clawkeepms = 0;
bool isOpen = true;
bool isNotFinished = false;
create_action_ptr(claw);

void stopClaw(){
  _hold_claw = false;
  clawgrp().move(0);
  isNotFinished = false;
}
void holdClaw(){
  _hold_claw = true;
}
void releaseClaw(){
  _hold_claw = false;
}
void HOLD_CLAW(void* param){
  while(true){
    if(isOpen){
      clawgrp().move(10);
    }else{
      clawgrp().move(-10);
    }
  }
}
void KEEP_CLAW_SERVICE(void* param){
  wait(clawkeepms);
  stopClaw();
}
void keepClaw(int speed, int waitms = 0){
  stopClaw();
  if(waitms == 0){
    clawgrp().move(speed);
  }else{
    Action claw{0};
    clawptr = &claw;
    component_type_e_t = Claw;
    isNotFinished = true;
    clawkeepms = waitms;
    clawgrp().move(speed);
    pros::Task keep_claw_service(KEEP_CLAW_SERVICE);
  }
}
void OPEN_SERVICE(void* param){
  float pos;
  clawptr->timer.startTimer();
  while (1){
    pos = clawgrp().position();
    if(pos >= CLAW_MAX_LIMIT){
      stopClaw();
      clawptr->timer.stopTimer();
      break;
    }
    clawptr->_value = pos;
    wait(5);
  }
  clawptr->_end = true;
}
void CLOSE_SERVICE(void* param){
  float pos;
  clawptr->timer.startTimer();
  while (1){
    pos = clawgrp().position();
    if(pos <= CLAW_MIN_LIMIT){
      stopClaw();
      clawptr->timer.stopTimer();
      break;
    }
    clawptr->_value = pos;
    wait(5);
  }
  clawptr->_end = true;
  isNotFinished = false;
}

Action openClaw(int speed){
  Action claw{int(clawgrp().position())};

  speed = abs(speed);

  clawgrp().move(speed);

  isNotFinished = true;

  isOpen = true;

  clawptr = &claw;

  component_type_e_t = Claw;

  pros::Task open_service(OPEN_SERVICE);

  return claw;
}
Action closeClaw(int speed){
  Action claw{int(clawgrp().position())};

  speed = abs(speed);

  clawgrp().move(-speed);

  isNotFinished = true;

  isOpen = false;

  clawptr = &claw;

  component_type_e_t = Claw;

  pros::Task close_service(CLOSE_SERVICE);

  return claw;
}
void initClaw(){
  pros::Task hold_claw(HOLD_CLAW);
}
