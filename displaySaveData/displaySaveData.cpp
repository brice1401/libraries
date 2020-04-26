#include "displaySaveData.h"
#include "Adafruit_LiquidCrystal.h"
#include "weatherStation.h"
#include <Arduino.h>
#include "RTClib.h"
#ifndef _SD_H_
#define _SD_H_
#include <SD.h>
#endif


/********************************************************************************************************
   Function for time management
/********************************************************************************************************/


unsigned long getUnixTimeM(DateTime instant){
  unsigned long minutes;
  minutes = (unsigned long) (instant.unixtime()/60UL);
  return(minutes);
}

int DurationLastSend(unsigned long start, DateTime instant){
  int duration;
  duration = int ((unsigned long) (instant.unixtime()/60UL) - start);
  return(duration);
}
unsigned long getUnixTimeS(DateTime instant){
  unsigned long minutes;
  minutes = (unsigned long) (instant.unixtime());
  return(minutes);
}

int DurationLastSendS(unsigned long start, DateTime instant){
  int duration;
  duration = int ((unsigned long) (instant.unixtime()) - start);
  return(duration);
}

String getDate(DateTime instant) {
  int Year = instant.year();
  int Month = instant.month();
  int Day = instant.day();
  String Date = String(Day) + '/' + String(Month) + '/' + String(Year);
  return(Date);
}
String getHoraireHM(DateTime instant){
  int Hour = instant.hour();
  int Minute = instant.minute();
  String Horaire = String(Hour) + ":" + String(Minute);
  return(Horaire);
}
String getMomentDatalog(DateTime instant){
  String moment = getDate(instant) + " " + getHoraireHM(instant) + ";";
  return(moment);
}

/*******************************************************************************************************
   Function for LCD Display
/*******************************************************************************************************/

String MessageLCD0; //message on upper line
String MessageLCD1; //message on lower line


void displaySelection(int numero, WeatherStation maStationMeteo){

  switch(numero){
    case 0:
      MessageLCD0 = "Temperature DHT:";
      MessageLCD1 = String(maStationMeteo.getTempDHT()) + " °C";
      break;

    case 1:
      MessageLCD0 = "Humidite :";
      MessageLCD1 = String(maStationMeteo.getHumidity()) + " %";
      break;

    case 2:
      MessageLCD0 = "Temp BMP :";
      MessageLCD1 = String(maStationMeteo.getTempBMP()) + " °C";
      break;

    case 3:
      MessageLCD0 = "Pression :";
      MessageLCD1 = String(maStationMeteo.getPressure()) + " hPa";
      break;
    case 4:
      MessageLCD0 = "Temperature RTC:";
      MessageLCD1 = String(maStationMeteo.getTempRTC()) + " °C";
      break;
    case 5:
      MessageLCD0 = "Luminosite :";
      MessageLCD1 = String(maStationMeteo.getLight()) + " Lux";
      break;
    case 6:
      MessageLCD0 = "Lumiere rouge :";
      MessageLCD1 = String(maStationMeteo.getLightRed()) + " ?";
      break;
    case 7:
      MessageLCD0 = "Lumiere verte :";
      MessageLCD1 = String(maStationMeteo.getLightGreen()) + " ?";
      break;
    case 8:
      MessageLCD0 = "Lumiere bleue :";
      MessageLCD1 = String(maStationMeteo.getLightBlue()) + " ?";
      break;
    case 9:
      MessageLCD0 = "Point de Rosee :";
      MessageLCD1 = String(dewPoint(maStationMeteo.getTempDHT(), maStationMeteo.getHumidity())) + " °C";
      break;
    case 10:
      MessageLCD0 = "Heat index :";
      MessageLCD1 = String(heatIndex(maStationMeteo.getTempDHT(), maStationMeteo.getHumidity())) + " °C";
      break;
  }
}

void lcdDisplay(Adafruit_LiquidCrystal lcd){
  lcd.setBacklight(HIGH);
  lcd.display();
  lcd.setCursor(0, 0);
  lcd.println("                "); //erase caractere in memory
  lcd.setCursor(0, 0);
  lcd.println(MessageLCD0);
  lcd.setCursor(0, 1);
  lcd.println("                ");
  lcd.setCursor(0, 1);
  lcd.println(MessageLCD1);
}

void lcdNoDisplay(Adafruit_LiquidCrystal lcd){
  lcd.noDisplay();
  lcd.setBacklight(LOW);
}
void displayDataSerial(WeatherStation maStationMeteo, DateTime instant){
  // display the data on the serial

  Serial.println("*************************************************");
  Serial.print(F("Mesure à  "));
  Serial.println(getMomentDatalog(instant));
  Serial.println("*************************************************");
  Serial.print(F("Pluie :  "));
  Serial.println(maStationMeteo.getRain());
  Serial.print(F("Vitesse vent :  "));
  Serial.println(maStationMeteo.getWindSpeed());
  Serial.print(F("Direction du vent :  "));
  Serial.println(maStationMeteo.getWindDir());
  Serial.print(F("Température DHT22 :  "));
  Serial.println(maStationMeteo.getTempDHT());
  Serial.print(F("Humidité :  "));
  Serial.println(maStationMeteo.getHumidity());
  Serial.print(F("Pression :  "));
  Serial.println(maStationMeteo.getPressure());
  Serial.print(F("Température BMP :  "));
  Serial.println(maStationMeteo.getTempBMP());
  Serial.print(F("Température RTC :  "));
  Serial.println(maStationMeteo.getTempRTC());
  Serial.print(F("Lumière claire :  "));
  Serial.println(maStationMeteo.getLight());
  Serial.print(F("Lumière rouge :  "));
  Serial.println(maStationMeteo.getLightRed());
  Serial.print(F("Lumière verte :  "));
  Serial.println(maStationMeteo.getLightGreen());
  Serial.print(F("Lumière bleue :  "));
  Serial.println(maStationMeteo.getLightBlue());

  Serial.println("*************************************************");
  Serial.print(F("Message radio : "));
  for(int j=0; j < 62; j++){
    Serial.print(maStationMeteo._radioBuffer[j], HEX);
  }
  Serial.println("");
  Serial.println("*************************************************");
}




/*******************************************************************************************************
   Function for datalogging on SD card
/*******************************************************************************************************/

void writeDataSD(String Filename, WeatherStation maStationMeteo, DateTime instant){

  File dataFile = SD.open(Filename, FILE_WRITE);

  if(dataFile){
    // the file is available, we can write on it
    String instantDataLog = getMomentDatalog(instant);


    dataFile.print(F("Rain; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getRain());

    dataFile.print(F("Wind Speed; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getWindSpeed());

    dataFile.print(F("Wind Direction; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getWindDir());

    dataFile.print(F("Temp DHT22; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getTempDHT());

    dataFile.print(F("Humidity; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getHumidity());

    dataFile.print(F("Pressure; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getPressure());

    dataFile.print(F("Temperature BMP; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getTempBMP());

    dataFile.print(F("Temperature RTC; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getTempRTC());

    dataFile.print(F("Light; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getLight());

    dataFile.print(F("Light Red; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getLightRed());

    dataFile.print(F("Light Green; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getLightGreen());

    dataFile.print(F("Light Blue; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getLightBlue());

    dataFile.print(F("Dew Point; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo._dewPoint);

    dataFile.print(F("Heat Index; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo._heatIndex);


    dataFile.print(F("Icing Point; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo._icingPoint);

    dataFile.print(F("Wind Chill; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo._windChill);

    dataFile.print(F("RSSI; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo._RSSI);

		dataFile.print(F("Battery Station voltage; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo.getBatteryVoltage());

		dataFile.print(F("Battery Receiver voltage; "));
    dataFile.print(instantDataLog);
    dataFile.println(maStationMeteo._batteryReceiverVoltage);





    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println(F("error opening datalog.txt"));
  }
}