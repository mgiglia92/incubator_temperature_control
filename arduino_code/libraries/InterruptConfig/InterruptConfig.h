/*
This is an experimental way to represent interrupt functions and variables that are edited inside the interrupt.
That way I can have different interrupt config files, that I can easily pick and place into arduino sketches.
Call the interrupt_initialization() function in void setup() to have the interrupts setup.
*/

#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

//Check preprocessor directive to choose correct TempControl class type
//This directive should be defined in the .ino file
#if HEATER_TYPE == 1
  extern TempControlPan t;
#elif HEATER_TYPE == 0
  extern TempControl t;
#endif

//Debouncing state machine for inexpensive digital encoder
int state=0; //State of debounce state machine
int prev_state; //previous state of the button
unsigned long prev_time; //time when interrupt was first triggered
unsigned long cur_time; //current time
unsigned long debounce_duration = 10; //ms

bool debounce_check()
{
  switch(state){
    case 0: //Initial state
      state=1;
      break;
    case 1: //Rising edge detected, start debounce timer
      prev_time = millis();
      state = 2;
      break;
    case 2: //Check after duration that pin is still high
      if((cur_time - prev_time) > debounce_duration)
      {
        state = 0;
        return true;
      }
      break;
    default:
      return false;
      break;
  }

}

void do_count()
{
  cur_time = millis();
  if(debounce_check())
  {
    if(digitalRead(3)){ t.control_data.T_incubator_ref += 0.5; }
    else { t.control_data.T_incubator_ref -= 0.5; }
  }
}


void interrupt_initialization()
{
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), do_count, LOW);
}