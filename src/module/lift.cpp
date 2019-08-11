#include "_header.h"


bool _hold_lift = 0, compare;
int liftkeepms = 0;
float liftTick = 0;
create_action_ptr(lift);

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
void KEEP_LIFT_SERVICE(void* param){
  wait(liftkeepms);
  stopLift();
}
void keepLift(int speed, int waitms = 0){
  stopLift();
  if(waitms == 0){
    liftgrp().move(speed);
  }else{
    liftTick = 1;
    liftkeepms = waitms;
    liftgrp().move(speed);
    pros::Task keep_lift_service(KEEP_LIFT_SERVICE);
  }
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
void initLift(){
  pros::Task hold_lift(HOLD_LIFT);
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
