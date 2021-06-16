#ifndef TEMPCONTROL_H
#include <TempControl.h>
#define TEMPCONTROL_H
#endif
#ifndef PID_H 
#define PID_H 
#include <PID_beard.h> 
#endif
#ifndef OLED_SCREEN_H
#include <OLED_Screen.h>
#define OLED_SCREEN_H
#endif
#ifndef STEPINPUT_H
#define STEPINPUT_H
#include <StepInput.h>
#endif
#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H
#include <SerialComms.h>
#endif
#ifndef INTERRUPTCONFIG_H
#define INTERRUPTCONFIG_H
#include <InterruptConfig.h>
#endif

//Generate struct to init PIDControl class
struct PID_Initializer pid_init = {
  kp:10,
  ki:0.1,
  kd:0,
  lowerLimit:0,
  upperLimit:200,
  sigma:0.1,
  Ts:1.0,
  flag:true,
};

//Generate strucut to initialize control data
struct ControlData data = {
  T_incubator_ref:30.0,
  T_heater_ref:60.0,
  T0:25.0,
  T1:23.3,
  T2:24.2,
  T3:19,
  H0: 0,
  F0: 255,
  error_type:0,
};

//Create Singletons
PIDControl controller(pid_init);
TempControl t(pid_init);
OLED_Screen screen;
StepInput step_input(0.250, 2000);
SerialComms com; //Create serial comms class, print data every 0.25 seconds

void setup(){
  Serial.begin(115200);
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
  com.handle_command();
  t.do_step_test(&com);
//  t.do_controls();
  screen.populate_info(t.control_data);
}
