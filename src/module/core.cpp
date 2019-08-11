#include "_header.h"

component_type_e component_type_e_t;
void wait(int ms){
  pros::delay(ms);
}
Motor_Group::Motor_Group(std::initializer_list<pros::Motor> port_set)
{
  this->motor_vector = std::vector(port_set);
}
pros::Motor Motor_Group::get(short idx)
{
  return this->motor_vector[idx];
}
short Motor_Group::size()
{
  return this->motor_vector.size();
}
void Motor_Group::move(short speed)
{
  for(int i = 0;i<motor_vector.size();i++)
  {
    this->motor_vector[i].move(speed);
  }
  this->speed = speed;
}
float Motor_Group::position()
{
  float total = 0;
  for(int i = 0;i<size();i++)
  {
    total += this->motor_vector[i].get_position();
  }
  return total / this->size();
}
float Motor_Group::position(short idx){
  return this->motor_vector[idx].get_position();
}
void Motor_Group::tare()
{
  for(int i = 0;i<this->size();i++)
  {
    this->motor_vector[i].tare_position();
  }
}


void Timer::Timer_Service() {
  timerRunning = true;
  this->time = 0;
    while(timerRunning){
      wait(10);
      this->time += 10;
    }
}

void Timer::startTimer() {
  pros::Task timerservice(
     this->Timer_Service_Delegate,
     reinterpret_cast<void*>(this)
  );
}

void Timer::stopTimer() {
  this->timerRunning = false;
}

int Timer::getTime() {
  return this->time;
}





int Action::_fnc_update()
{
  return _value;
}


void Action::_fnc_to(int value,std::function<void()> event)
{
  fnc_event_to[value] = event;
}


void Action::handler_to()
{
  int times_called = 0;
  while(1)
  {
    if (fnc_event_to[_value])
    {
      fnc_event_to[_value]();
      times_called++;
    }
    if (times_called >= fnc_event_to.size())
    {
      break;
    }
    wait(5);
  }
}


void Action::handler_finish()
{
  int counter = 0;
  while(1){
    counter++;
    if (_end){
      fnc_event_finish();
      break;
    }
    wait(5);
  }
}

extern int moveTick,liftTick,rollerTick;
extern bool isNotFinished;
void Action::handler_run()
{

    /*//to
    if(!fnc_event_to.empty())
    {
      pros::Task toservice(
         this->Action_To_Delegate,
         reinterpret_cast<void*>(this)
      );
    }

    //finish
    if(_end_activated)
    {
      pros::Task finishservice(
        this->Action_Finish_Delegate,
        reinterpret_cast<void*>(this)
      );
    }*/

    //await Until
    if (_wait_until_value != -1 || _wait_until_time != -1)
    {
      /**********
       only need to wait for value
      **********/
      if (_wait_until_value != -1 && _wait_until_time == -1)
      {
        //if start value is less that wait value (which means the value of the sensor should be incrementing)
        if (_start_value < _wait_until_value){
          while(1){

            if (_value > _wait_until_value){
              break;
            }
            wait(5);
          }
        }


        //if start value is more that wait value (which means the value of the sensor should be decrementing)
        if (_start_value > _wait_until_value){
          while(1){
            if (_value < _wait_until_value){
              break;
            }
            wait(5);
          }
        }

      }


      /**********
       only need to wait for time
      **********/
      else if (_wait_until_time != -1 && _wait_until_value == -1)
      {
        while(1){
          if (timer.getTime()>_wait_until_time){
            break;
          }
          wait(5);
        }

      }
      /**********
       need to wait for both(if one of them is satisfied it will break)
      **********/
      else
      {
        //if start value is less that wait value (which means the value of the sensor should be incrementing)
        if (_start_value < _wait_until_value){
          while(1){
            if (_value > _wait_until_value||timer.getTime()>_wait_until_time){
              break;
            }
            wait(5);
          }
        }


        //if start value is more that wait value (which means the value of the sensor should be decrementing)
        if (_start_value > _wait_until_value){
          while(1){
            if (_value < _wait_until_value||timer.getTime()>_wait_until_time){
              break;
            }
            wait(5);
          }
        }
      }
    }

    //if no need to awaitUntil, lock the thread Until value satisfied
    if(_wait_until_value == -1 && _wait_until_time == -1)
    {
      switch (component_type_e_t)
      {
        case Drive:
        while(moveTick != 0){
          wait(5);
        }
        break;

        case Lift:
        while(liftTick != 0){
          wait(5);
        }
        break;

        case Roller:
        while(rollerTick != 0){
          wait(5);
        }
        break;

        case Claw:
        while(isNotFinished){
          wait(5);
        }
        break;
      }
    }
}

Action::Action(int startValue)
{
  _start_value = startValue;
}

Action& Action::awaitUntil(int value,int limit)
{
  _wait_until_value = value;
  _wait_until_time = limit;
  return *this;
}
Action& Action::to(int value ,std::function<void()> event)
{
  fnc_event_to[value] = event;
  return *this;
}
Action& Action::finish(std::function<void()> event){
  fnc_event_finish = event;
  _end_activated = true;
  return *this;
}

extern_action_ptr(drive);
extern_action_ptr(lift);
extern_action_ptr(roller);
extern_action_ptr(claw);

void _lockthread()
{
  switch (component_type_e_t)
  {
    case Drive:
    drive().handler_run();
    break;

    case Lift:
    lift().handler_run();
    break;

    case Roller:
    roller().handler_run();
    break;

    case Claw:
    claw().handler_run();
    break;
  }
}
void waitUntilAllFinished(){
  while(moveTick!=0||liftTick!=0){
    wait(5);
  }
}
