#include "zeltapi.h"
#include "autoplan/plan1.h"
#include "autoplan/plan2.h"
#include "autoplan/plan3.h"
#include "autoplan/plan4.h"
#include "autoplan/oneMinute.h"
/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void disabled(){
    lcd_print(0,"auto plan: %d",autoSelect);
  if(isBlueSide){
    lcd_print(1,"side: blue");
  }else{
    lcd_print(1,"side: red");
  }
}
void autonomous() {
  switch (autoSelect){
    case 1:
      plan1();
    break;

    case 2:
      plan2();
    break;

    case 3:
      plan3();
    break;

    case 4:
      plan4();
    break;

    case 5:
      plan5();
    break;
  }
}
