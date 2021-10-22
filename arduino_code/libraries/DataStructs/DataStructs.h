struct ControlData{
    double T_incubator_ref; //Incubator desired temp
    double T_heater_ref; //Silicone heater desired temp
    double T0; //Temp on thermistor 0
    double T1; 
    double T2;
    double T3;
    int H0; //Heater 0 duty cycle
    int H1; //Heater 1 duty cycle
    int F0;
    int error_type;
};