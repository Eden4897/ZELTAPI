/**
 * \file module/lift.cpp
 *
 * all lift commands and functions
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
/*********************
 *    INCLUDES
 ********************/
#include "_header.h"

/*********************
 *	    CONFIG
 ********************/
bool _hold_lift = 0;
bool compare;
int liftkeepms = 0;
float liftTick = 0;

create_action_ptr(lift);

void stopLift();
void holdLift();
/*********************
 *      TASKS
 ********************/

void KEEP_LIFT_SERVICE(void* param){
  wait(liftkeepms);
  stopLift();
}


void HOLD_LIFT(void* param){
  while(true)
  {
    if(_hold_lift)
    {
      float absPos = liftgrp().position();

      while(_hold_lift)
      {
        liftgrp().move(absPos-liftgrp().position());
        wait(5);
      }
    }
    wait(5);
  }
}


void LIFT_SERVICE(void *param){
  float pos;
  liftptr->timer.startTimer();

  while(1){
    pos = liftgrp().position();
    if (!compare){
      if (liftTick>liftgrp().position()){
        holdLift();
        liftptr->timer.stopTimer();
        break;
      }
    }else{
      if (liftTick<liftgrp().position()){
        holdLift();
        liftptr->timer.stopTimer();
        break;
      }
    }
    liftptr->_value = pos*360/900;
    wait(5);
  }
  liftptr->_end = true;
}


/*********************
 *	CORE FUNCTIONS
 ********************/
void initLift(){
  pros::Task hold_lift(HOLD_LIFT);
}


void holdLift(){
  liftTick = 0;
  _hold_lift = true;
}


void releaseLift(){
  _hold_lift = false;
}


void stopLift(){
  releaseLift();
  liftTick = 0;
  liftgrp().move(0);
}


/*********************
 *	USER FUNCTIONS
 ********************/
void keepLift(int speed, int waitms = 0){
  stopLift();
  if(waitms == 0){
    liftgrp().move(speed);
  }else{
    Action lift{0};
    liftptr = &lift;
    component_type_e_t = Lift;
    liftTick = 1;
    liftkeepms = waitms;
    liftgrp().move(speed);
    pros::Task keep_lift_service(KEEP_LIFT_SERVICE);
  }
}


Action moveLift(int targetDeg, int speed){
  Action lift{int(liftgrp().position())};

  float targetTick = targetDeg*900/360;

  targetTick = targetTick > LIFT_MAX_LIMIT? LIFT_MAX_LIMIT:targetTick;
  targetTick = targetTick < LIFT_MIN_LIMIT? LIFT_MIN_LIMIT:targetTick;

  stopLift();

  compare = liftgrp().position()<targetTick;

  liftTick = targetTick;

  if(compare){
    liftgrp().move(speed);
  }else{
    liftgrp().move(-speed);
  }

  liftptr = &lift;

  component_type_e_t = Lift;

  pros::Task lift_service(LIFT_SERVICE);

  return lift;
}
