/*
TempControlPan is a class that will handle controlling the temperature of the incubator.
It will contain pins for 4 temperature sensors: 
T0: Silicone Heating pad temp
T1: Incubator temp 1 (middle of incubator)
T2: Incubator temp 2 (top of incubator)
T3: Ambient temperature

One heating element controller through an SSR
H0:

Cascade loop control of the inside temperature.

*/
#ifndef WDT_H
#include <avr/wdt.h>
#define WDT_H
#endif

#ifndef PID_H
#include "PID_beard.h"
#define PID_H
#endif

#ifndef OLED_SCREEN_H
#include <OLED_Screen.h>
#define OLED_SCREEN_H
#endif

#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H
#include <DataStructs.h>
#endif

#ifndef HARDWARECONFIG_H
#define HARDWARECONFIG_H
#include <hardware_config.h>
#endif

#ifndef STEPINPUT_H
#define STEPINPUT_H
#include <StepInput.h>
#endif

#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H
#include <SerialComms.h>
#endif

class TempControlPan:public PIDControl{

    public:
//Constructor
    TempControlPan(struct PID_Initializer p, struct PID_Initializer p2):PIDControl(p),pid2(p2)
    {   
        ledPin = 13;
        // wdt_enable(WDTO_4S);
        // wdt_reset();

        //Initialize pins
        T0_pin = T0_PIN;
        T1_pin = T1_PIN;
        T2_pin = T2_PIN;
        T3_pin = T3_PIN;
        H0_pin = H0_PIN;
        F0_pin = F0_PIN;

        //Other important PID values, hardcoded for now!
        //TODO: Add these to struct!
        anti_windup_activated = 1;

        current=micros();
        prev=current;
        
    };
//Variables
    int ledPin;
    //Pin connections
    int T0_pin;
    int T1_pin;
    int T2_pin;
    int T3_pin;
    int H0_pin;
    int F0_pin;
    int screen_SCL;
    int screen_SDA;

    //Safety variables?

    //Timing variables
    unsigned long current;
    unsigned long prev;

    //Control data for system
    struct ControlData control_data;
    //Extra PID controller for second heater
    PIDControl pid2;

//Methods
    void initialize();
    void init_control_data(ControlData);

    //Update temperatures
    void update_temps();

    //Check safety (return error code)
    void check_safety();

    //Do controls
    void do_controls();

    //convert raw analog read into deg C
    double convert_to_C(int, double, double, double);

    //Do a small step input to the system
    void do_debug_mode(SerialComms *comms);

    //Check dt
    bool check_dt();
};

