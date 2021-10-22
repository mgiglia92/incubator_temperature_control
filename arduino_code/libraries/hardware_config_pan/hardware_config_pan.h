//TODO: ADD SECOND HEATER PINS
//TODO: Double check nominal and series resistances
#define T0_PIN 0 //Thermistor analog pin#
#define T1_PIN 1 //Thermistor analog pin#
#define T2_PIN 2 //Thermistor analog pin#
#define T3_PIN 3 //Thermistor analog pin#
//---------------------------------------------
#define H0_PIN 9 //Heater 0 pwm pin
#define H1_PIN 11 //Heater 1 pwm pin
#define F0_PIN 10

#define AREF_VOLTAGE 3.3 // Reference voltage

//Thermistor parameters
#define TN 25.0 //Nominal temperature for thermistors
#define MINTEMP 15.0 //The minimum tmperature any thermistor should see (deg C)
#define MAXTEMP 140.0 //The maximum temp any thermistor should see (deg C)

#define RN0 100000.0 //Nominal resistance for Thermistor0 which is on Heater 0
#define RS0 10000.0 //Series resistance for Thermistor0
#define B0 3950.0 //Beta value for Thermistor0

#define RN1 100000.0 //Nominal resistance for Thermistor1 which is on Heater 1
#define RS1 10000.0 //Series resistance for Thermistor1
#define B1 3950.0 //Beta value for Thermistor1

#define RN2 10000.0 //Nominal resistance for Thermistor2 inside the pan
#define RS2 10000.0 //Series resistance for Thermistor2
#define B2 3950.0 //Beta value for Thermistor2

#define RN3 10000.0 //Nominal resistance for Thermistor3 inside the pan
#define RS3 10000.0 //Series resistance for Thermistor3
#define B3 3950.0 //Beta value for Thermistor3


#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

