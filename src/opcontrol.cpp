#include "zeltapi.h"
pros::Controller master(pros::E_CONTROLLER_MASTER);
void opcontrol() {
	pidLeftDrive().setUp(900, 90, 450);
	while(1){
		wait(5);
		leftDrive().move(pidLeftDrive().calculate(leftDrive().position()));
		lcd_print(0,"error: %d",pidLeftDrive()._error);
		lcd_print(1,"pos: %d",leftDrive().position());
		lcd_print(2,"_target: %d",pidLeftDrive()._target);
		lcd_print(3,"_out: %d",pidLeftDrive()._output);
	}
}
