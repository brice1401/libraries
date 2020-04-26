/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BH1745NUC
        This code is designed to work with the BH1745NUC_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=BH1745NUC_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include "BH1745NUC.h"

BH1745NUC bh;

void setup(void) 
{
    Serial.begin(9600);
    
    
    // Initialize the sensor for the color measurements
    bh.Initialize();

    bh.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = bh.bh_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint16_t red, green, blue, clear;

        Serial.println("Getting Color Readings from BH1745NUC");
        Serial.println(" ");
        // Set Up the Color Sensor
        bh.Initialize();
        delay(500);
        // Read and print out the colors in lux values
        red = bh.getRedColor();
        green = bh.getGreenColor();
        blue = bh.getBlueColor();
        clear = bh.getClearColor();

        // Output data to screen
        Serial.print("Red Color Illuminance: ");
        Serial.println(red);
        Serial.print("Green Color Illuminance: ");
        Serial.println(green);
        Serial.print("Blue Color Illuminance: ");
        Serial.println(blue);
        Serial.print("Clear Color Illuminance: ");
        Serial.println(clear);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("BH1745NUC Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
