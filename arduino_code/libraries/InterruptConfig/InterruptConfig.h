/*
This is an experimental way to represent interrupt functions and variables that are edited inside the interrupt.
That way I can have different interrupt config files, that I can easily pick and place into arduino sketches.
Call the interrupt_initialization() function in void setup() to have the interrupts setup.
*/

#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#ifndef TEMPCONTROL_H
#define TEMPCONTROL_H
#include <TempControl.h>
#endif

extern TempControl t;


void do_count()
{
    if(digitalRead(3) == digitalRead(2)){ t.control_data.T_incubator_ref -= 1.0; }
    else { t.control_data.T_incubator_ref += 1.0; }
}

void interrupt_initialization()
{
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), do_count, RISING);
}