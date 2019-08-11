#include "zeltapi.h"
void initialize(){
  llemu::init();
  wait(1000);
  initMove();
  initLift();
}
