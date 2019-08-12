#include "_header.h"

bool _hold_roller = 0;
int rollerkeepms = 0;
float rollerTick = 0;
create_action_ptr(roller);

void stopRoller(){
  _hold_roller = false;
  rollergrp().move(0);
  rollerTick = 0;
}
void holdRoller(){
  _hold_roller = true;
}
void releaseRoller(){
  _hold_roller = false;
}
void HOLD_ROLLER(void* param){
  while(true)
  {
    if(_hold_roller)
    {
      float absPos = rollergrp().position();

      while(_hold_roller)
      {
        rollergrp().move(absPos-rollergrp().position());
        wait(5);
      }
    }
    wait(5);
  }
}
void KEEP_ROLLER_SERVICE(void* param){
  wait(rollerkeepms);
  stopRoller();
}
void keepRoller(int speed,int waitms = 0){
  stopRoller();
  if(waitms == 0){
    rollergrp().move(speed);
  }else{
    Action roller{0};
    rollerptr = &roller;
    component_type_e_t = Roller;
    rollerTick = 1;
    rollerkeepms = waitms;
    rollergrp().move(speed);
    pros::Task keep_roller_service(KEEP_ROLLER_SERVICE);
  }
}
void ROLLER_SERVICE(void* param){
  float pos;
  rollerptr->timer.startTimer();

  while(1){
    pos = rollergrp().position();
    lcd_print(2,"rollerTick: %d",rollerTick);
    lcd_print(3,"pos: %d",pos);
    if (rollergrp().position()>rollerTick){
      stopRoller();
      rollerptr->timer.stopTimer();
      break;
    }
    rollerptr->_value = pos/900;
    wait(5);
  }
  rollerptr->_end = true;
}
Action moveRoller(float revolutions, int speed){
  Action roller{0};

  stopRoller();

  rollergrp().tare();

  rollerTick = revolutions*900;

  rollergrp().move(speed);

  rollerptr = &roller;

  component_type_e_t = Roller;

  pros::Task roller_service(ROLLER_SERVICE);

  return roller;
}
void initRoller(){
  pros::Task hold_roller(HOLD_ROLLER);
}
