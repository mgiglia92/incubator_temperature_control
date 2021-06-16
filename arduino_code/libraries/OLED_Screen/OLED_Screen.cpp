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

#ifndef OLED_SCREEN_H
#include <OLED_Screen.h>
#define OLED_SCREEN_H
#endif

OLED_Screen::initialize()
{
        begin(SSD1306_SWITCHCAPVCC, 0x3C);
        display();
        clearDisplay();

}

OLED_Screen::create_bounding_box()
{
    drawRect(0, 8, 124, SCREEN_HEIGHT-7, SSD1306_WHITE);
    display();
}


void OLED_Screen::populate_info(ControlData data)
{   
    //Required to print text
    setTextSize(1);
    setTextColor(WHITE);
    //Clear display
    clearDisplay();
    // create_bounding_box();

    //Print error on top
    setCursor(0,0);
    print("Error: "); println(data.error_type); 
    
    //Print right side data fist
    setCursor(64, 9);
    print("H0:"); print(data.H0);
    setCursor(64, 9+8);
    print("Tinc:"); print(data.T_incubator_ref);
    setCursor(64, 9+8+8);
    print("Tamb:"); print(data.T3);


    //Print left side data
    setCursor(0,9);
    print("T0:"); println(data.T0,2);
    print("T1:"); println(data.T1,2);
    print("T2:"); println(data.T2,2);

    //Display the contents
    display();
}