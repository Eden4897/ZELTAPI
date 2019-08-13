/**
 * \file zeltapi/module/core.hpp
 *
 * core header
 *
 * Copyright (c) 2019-2020, by Eden Cheung.
 * All rights reserved
 */
#include "main.h"

#ifndef _ZELTAPI_CORE_HPP_
#define _ZELTAPI_CORE_HPP_
#include <vector>
#include <map>
#include <functional>
#include <initializer_list>

enum component_type_e{
  Drive,Lift,Roller,Claw
};

void wait(int ms);


struct Motor_Group{
  std::vector<pros::Motor> motor_vector;

  int speed = 0;

  Motor_Group(std::initializer_list<pros::Motor> port_set);

  pros::Motor get(short idx);

  short size();

  void move(short speed);

  float position();

  float position(short idx);

  void tare();
};


struct Timer
{
  bool timerRunning;
  int time;

  static void Timer_Service_Delegate(void* param) {
     Timer* ptr = reinterpret_cast<Timer*>(param);
     ptr->Timer_Service();
  }

  void Timer_Service();

  void startTimer();

  void stopTimer();

  int getTime();
};


struct Action
{
  std::string _handler_id = "*";

  int _value = 0;

  int _wait_until_value = -1;

  int _wait_until_time  = -1;

  int _start_value = 0;

  bool _end = false;

  //means is the fnc_event_finish used
  bool _end_activated = false;

  Action(int startValue);

  Timer timer = Timer();

  Action& awaitUntil(int value = -1,int limit = -1);

  Action& to(int value,std::function<void()> event);

  Action& finish(const std::function<void()> event);

  std::map<int,std::function<void()>> fnc_event_to;

  std::function<void()> fnc_event_finish = [](){};

  int _fnc_update();

  void _fnc_to(int value,std::function<void()> event);

  void handler_run();

  void handler_to();

  void handler_finish();

  static void Action_To_Delegate(void* param) {
     Action* ptr = reinterpret_cast<Action*>(param);
     ptr->handler_to();
  }
  static void Action_Finish_Delegate(void* param) {
     Action* ptr = reinterpret_cast<Action*>(param);
     ptr->handler_finish();
  }
};


void _lockthread();

void waitUntilAllFinished();


#define create_motor_group(x,y...) Motor_Group& x()\
{\
  static Motor_Group* ptr = new Motor_Group({y});\
  return *ptr;\
}

#define extern_motor_group(x) Motor_Group& x()


#define create_pid(x,a,b,c,d,e) PID& x()\
{\
  static PID* ptr = new PID(a,b,c,d,e);\
  return *ptr;\
}

#define extern_pid(x) PID& x()


#define create_action_ptr(x) Action* x##ptr = nullptr;\
Action& x(){\
  static Action* ptr = new Action(0);\
  ptr = x##ptr;\
  return *ptr;\
}
#define extern_action_ptr(x) Action& x()


#define await ;_lockthread()


#define waitUntil(x) \
while(!x){\
  wait(5);\
}
#define redOnly(x) \
if(!isBlueSide){\
  x;\
}
#define blueOnly(x) \
if(isBlueSide){\
  x;\
}

#endif
