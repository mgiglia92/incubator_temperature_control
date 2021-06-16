#ifndef HARDWARECONFIG_H
#define HARDWARECONFIG_H
#include <hardware_config.h>
#endif


#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H
#include <DataStructs.h>
#endif

class StepInput
{
public:
    int sample_length;
    unsigned int *T0;
    unsigned int *Tamb;
    int current_index;
    double Ts;
    unsigned long current;
    unsigned long prev;

    bool started;
    bool finished;
    bool printed;

    StepInput(double, int);

    //methods
    bool check_dt();
    
    //method do step
    void record_step(struct ControlData data);

    //print data to serial monitor as CSV
    void print_data();
};