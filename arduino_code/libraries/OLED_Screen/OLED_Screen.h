#ifndef WIRE_H
#include <Wire.h>
#define WIRE_H
#endif

#ifndef GFX_H
#include <Adafruit_GFX.h>
#define GFX_H
#endif

#ifndef SSD1306_H
#include <Adafruit_SSD1306.h>
#define SSD1306_H
#endif

#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H
#include <DataStructs.h>
#endif

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


class OLED_Screen:public Adafruit_SSD1306{

    public:
    //Hard coded constructor
    OLED_Screen():Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
    {};
    //Initialize the screen (used because methods dont work in global variable space, so we call this in void setup)
    initialize();
    //Not currently used, creates a bounding box around the text
    create_bounding_box();
    //Populate the screen with the relevant info
    void populate_info(struct ControlData);
    //Raw print control data to serial monitor
    void raw_print(struct ControlData);
};

struct DisplayData{
    //Error
    char* error;
    //Temperature values
    double T0;
    double T1;
    double T2;
    double T3;
    //Heater duty cycle (0-255)
    int H0;
    //Fan duty cycle
    int F0;
};