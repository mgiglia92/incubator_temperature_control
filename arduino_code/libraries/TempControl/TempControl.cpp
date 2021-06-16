#ifndef WDT_H
#include <avr/wdt.h>
#define WDT_H
#endif

#ifndef PID_H
#define PID_H
#include "PID_beard.h"
#endif

#ifndef TEMPCONTROL_H
#define TEMPCONTROL_H
#include <TempControl.h>
#endif

#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

void TempControl::initialize()
{

}

//Take in raw ADC values, convert to deg C and return
//TODO: Do curve fitting for each sensor
double TempControl::convert_to_C(int raw, double rn, double rs, double b)
{
    double Vout = AREF_VOLTAGE*((raw/1023.0));
    double Vin = AREF_VOLTAGE;
    double R = (Vin*rs/Vout) - rs;
    double B = 3950.0;
    double To = 25 + 273.15;
    double Tk1 = (1/To) + (1/B)*(log(R/rn));
    return 1/Tk1 - 273.15;
}

// Pull the data from the analog ports, convert to temperatures
void TempControl::update_temps()
{
    control_data.T0 = convert_to_C(analogRead(T0_pin), RN0, RS0, B0);
    control_data.T1 = convert_to_C(analogRead(T1_pin), RN1, RS1, B1);
    control_data.T2 = convert_to_C(analogRead(T2_pin), RN2, RS2, B2);
    control_data.T3 = convert_to_C(analogRead(T3_pin), RN3, RS3, B3);
}

void TempControl::check_safety()
{
    control_data.error_type = 0;
    
    //Limit T incubator reference
    if(control_data.T_incubator_ref > 100.0) { control_data.T_incubator_ref = 100.0; control_data.error_type = 1;}

    //Check if thermistor sensor values are within reason
    if(control_data.T0 > MAXTEMP){ control_data.error_type = 1; }
    else if(control_data.T0 < MINTEMP) { control_data.error_type = 2; }

    if(control_data.T1 > MAXTEMP){ control_data.error_type = 1; }
    else if(control_data.T1 < MINTEMP) { control_data.error_type = 2; }

    if(control_data.T2 > MAXTEMP){ control_data.error_type = 1; }
    else if(control_data.T2 < MINTEMP) { control_data.error_type = 2; }

    if(control_data.T3 > MAXTEMP){ control_data.error_type = 1; }
    else if(control_data.T3 < MINTEMP) { control_data.error_type = 2; }
    
    //Check if thermistor circuit is connected to analog pins by temporarily turning on the internal pullup
//A0
    pinMode(T0_pin, INPUT_PULLUP);
    int check0 = analogRead(T0_pin);
    if(check0 < 10) { control_data.error_type = 3; } //If voltage is close to GND, then thermistor circuit is likely disconnected, throw error state
    pinMode(T0_pin, INPUT);
//A1
    pinMode(T1_pin, INPUT_PULLUP);
    int check1 = analogRead(T1_pin);
    if(check1 < 10) { control_data.error_type = 3; } //If voltage is close to GND, then thermistor circuit is likely disconnected, throw error state
    pinMode(T1_pin, INPUT);
//A2
    pinMode(T2_pin, INPUT_PULLUP);
    int check2 = analogRead(T2_pin);
    if(check2 < 10) { control_data.error_type = 3; } //If voltage is close to GND, then thermistor circuit is likely disconnected, throw error state
    pinMode(T2_pin, INPUT);
//A3
    pinMode(T3_pin, INPUT_PULLUP);
    int check3 = analogRead(T3_pin);
    if(check3 < 10) { control_data.error_type = 3; } //If voltage is close to GND, then thermistor circuit is likely disconnected, throw error state
    pinMode(T3_pin, INPUT);
    
    //Shut off heater if error_type != 0
    if(control_data.error_type != 0) { control_data.H0 = 0; }

    return control_data.error_type;
}

void TempControl::do_controls()
{    
    if(check_dt())
    {
        //Get readings
        update_temps();

        //Get Incubator temp error
        double Tavg = (control_data.T1 + control_data.T2 + control_data.T3)/3.0;
        double T_incubator_err = control_data.T_incubator_ref - Tavg;

        //Determine Heater Desired Temperature, clip when above 120.
        control_data.T_heater_ref = min(control_data.T_incubator_ref + T_incubator_err * 8.0, 120);

        //Determine Heater input based on T_heater_ref
        double heater_output = PID(control_data.T_heater_ref, control_data.T0);

        //Convert heater output (in desired watts?) to a duty cycle
        //Limit to 225 (slightly lower than the maximum PWM value)
        //Make limit slightly lower as a precaution
        control_data.H0 = max(min(heater_output, 225), 0);

        //Control heater pin duty cycle
        analogWrite(H0_pin, control_data.H0);

        //Check safety
        check_safety();
    }
}

void TempControl::do_debug_mode(SerialComms *comms)
{
    // update temps()
    if(check_dt())
    {
        update_temps();
        if(comms->start_step_test==1){
            // control_data.H0 = 75; //Set heater value to 75 (pwm value 0-255) to start step test
            do_controls();
            pinMode(H0_pin, OUTPUT);
            analogWrite(H0_pin, control_data.H0);
        }
        else if(comms -> start_step_test == 0){
            control_data.H0 = 0; //Set heater value to 0 since we're done
            analogWrite(H0_pin, control_data.H0);
        }
        comms->send_data(control_data);
    }
}

void TempControl::init_control_data(ControlData data)
{
    control_data = data;
}

bool TempControl::check_dt()
{
    current = micros();
    double dt = (current-prev)/1000000.0;
    if(dt>=Ts){ prev=current; return true;}
    return false;
}