#ifndef STEPINPUT_H
#define STEPINPUT_H
#include <StepInput.h>
#endif

#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif


StepInput::StepInput(double deltat, int length)
{
    Ts=deltat;
    current=0;
    prev=0;
    started=false;
    finished=false;
    printed = false;
    current_index=0;
    sample_length=length;
    delete T0;
    delete Tamb;
    T0 = new unsigned int[sample_length];
    Tamb = new unsigned int[sample_length];
}

bool StepInput::check_dt()
{
    current = micros();
    double dt = (current-prev)/1000000.0;
    if(dt>=Ts){ prev=current; return true;}
    return false;
}

void StepInput::record_step(ControlData data)
{

    //If we're finished, shut off heater, print data
    if(finished && !printed)
    {
        //Shut off heater
        analogWrite(H0_PIN, 0);
        // print_data();
        printed=true;
        // Serial.println("Finished, printing data!");
    }

    //If we haven't started and we're not finished, delay 1 second, turn on heater, set started to true
    if(!started && !finished)
    {
        // Serial.println("Starting step input response, waiting 1 second...");
        delay(1000);
        pinMode(H0_PIN, OUTPUT);
        analogWrite(H0_PIN, data.H0);
        started=true;
        // Serial.println("Heater turned on, starting sampling.");

        //Reset timing variables
        current = micros();
        prev = current;
    }

    //If we're started but not finished, record data point, increment index
    if(started && !finished)
    {
        T0[current_index] = data.T0*1000;
        Tamb[current_index] = data.T3*1000;
        current_index++;
        // Serial.println("Saved data point!");
        // Serial.println(current_index);
    }

}

void StepInput::print_data()
{
    Serial.print("Heater pwm:"); Serial.print(75); Serial.println();
    for(int i=0; i<sample_length; i++)
    {
        Serial.print(T0[i]); Serial.print(',');
        Serial.print(Tamb[i]); Serial.print(',');
        Serial.println();
    }
}