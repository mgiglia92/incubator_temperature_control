#define HEATER_TYPE 1 //Preprocessor directive to allow the extern call in InterruptConfig.h to choose which TempControl class to use, 1 is Pan, 0 is Incubator

#ifndef TEMPCONTROLPAN_H
#include <TempControlPan.h>
#define TEMPCONTROLPAN_H
#endif
#ifndef PID_H 
#define PID_H 
#include <PID_beard.h> 
#endif
#ifndef OLED_SCREEN_H
#include <OLED_Screen.h>
#define OLED_SCREEN_H
#endif
#ifndef INTERRUPTCONFIG_H
#define INTERRUPTCONFIG_H
#include <InterruptConfig.h>
#endif



//Generate struct to init PIDControl class
struct PID_Initializer pid_init = {
  kp:3,
  ki:1,
  kd:0,
  lowerLimit:0,
  upperLimit:255,
  sigma:0.1,
  Ts:1.0,
  flag:true,
};
struct PID_Initializer pid_init_2 = {
  kp:3,
  ki:1,
  kd:0,
  lowerLimit:0,
  upperLimit:255,
  sigma:0.1,
  Ts:1.0,
  flag:true,
};

//Generate strucut to initialize control data
struct ControlData data = {
  T_incubator_ref:30.0,
  T_heater_ref:50.0,
  T0:25.0,
  T1:23.3,
  T2:24.2,
  T3:19,
  H0: 155,
  H1:0,
  F0: 255,
  error_type:0,
};

//Create Singletons
//PIDControl controller(pid_init);
TempControlPan t(pid_init, pid_init_2);
OLED_Screen screen;


void setup(){
  Serial.begin(9600);
  Serial.print("void setup");
  analogReference(EXTERNAL);//Set the aref to be external
  pinMode(t.ledPin, OUTPUT);
  digitalWrite(t.ledPin, HIGH);
  delay(100);
  digitalWrite(t.ledPin, LOW);
  delay(100);   
  interrupt_initialization();
  screen.initialize();
  t.init_control_data(data);
}

void loop(){
  t.do_controls();
//  screen.populate_info(t.control_data);
  screen.raw_print(t.control_data);
}
