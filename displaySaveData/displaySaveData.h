#include <Adafruit_LiquidCrystal.h>
#include "weatherStation.h"
#include <Arduino.h>
#include "RTClib.h"


#ifndef _SD_H_
#define _SD_H_
#include <SD.h>
#endif

/*
 * this script is to display the data on a I2C LCD
 * it check for bouton pressing and display all the data
 */

void displaySelection(int numero, WeatherStation maStationMeteo);
void lcdDisplay(Adafruit_LiquidCrystal lcd);
void lcdNoDisplay(Adafruit_LiquidCrystal lcd);
void displayDataSerial(WeatherStation maStationMeteo, DateTime instant);

// function for time management
unsigned long getUnixTimeM(DateTime instant);
int DurationLastSend(unsigned long start, DateTime instant);
unsigned long getUnixTimeS(DateTime instant);
int DurationLastSendS(unsigned long start, DateTime instant);
String getDate(DateTime instant);
String getHoraireHM(DateTime instant);
String getMomentDatalog(DateTime instant);

// function for datalogging
void writeDataSD(String Filename, WeatherStation maStationMeteo, DateTime instant);
