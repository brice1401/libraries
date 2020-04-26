#include <math.h>
#include <Arduino.h>
#include "weatherStation.h"
#include <stdlib.h>;
#include "RTClib.h"



/* Creation of a WeatherStation class to store data */
/* This class will be useful for coding/decoding the message send via radio */

/*******************************************************************************************************/
/* Constructor and destructor */
/*******************************************************************************************************/

WeatherStation::WeatherStation()
{

  /*
   * Init all the pin of I/O
   * and activate the library of all the sensor
   */

  // init the attribute

  _rain = 0;
  _windDir = 0;
  _windSpeed = 0;
  _tempDHT = 0;
  _tempBMP = 0;
  _humidity = 0;
  _pressure = 0;
  _altitude = 0;
  _light = 0;
  _lightRed = 0;
  _lightGreen = 0;
  _lightBlue = 0;
  _batteryVoltage = 0;
  _batteryTemp = 0;
  _tempRTC = 0;
  _iconNameWindDir = "";
	_iconPressureForecast = "";

  _avgTemp = 0;
  _dewPoint = 0;
  _icingPoint = 0;
  _windChill = 0;
  _heatIndex = 0;

	_batteryReceiverVoltage = 0;
	_RSSI = 0;

  for(int i=0; i<7; i++){
    _rain7d[i] = 0;
  }
  for(int i=0; i<24; i++){
    _rain24h[i] = 0;
  }

}


WeatherStation::~WeatherStation() {
  //Destructor
}

/*******************************************************************************************************/
/* Function for get and set*/
/*******************************************************************************************************/

float WeatherStation::getRain() {
  return (_rain);
}
float WeatherStation::getRain24h() {
  float sumRain = 0;
  for(int i=0; i<24; i++){
    sumRain += _rain24h[i];
  }
  return (sumRain);
}
float WeatherStation::getRain7d() {
  float sumRain = 0;
  for(int i=0; i<7; i++){
    sumRain += _rain7d[i];
  }
  return (sumRain);
}
float WeatherStation::getWindDir() {
  return (_windDir);
}
float WeatherStation::getWindSpeed() {
  return (_windSpeed);
}
float WeatherStation::getTempDHT() {
  return (_tempDHT);
}
float WeatherStation::getTempBMP() {
  return (_tempBMP);
}
float WeatherStation::getHumidity() {
  return (_humidity);
}
float WeatherStation::getPressure() {
  return (_pressure);
}
float WeatherStation::getAltitude() {
  return (_altitude);
}
float WeatherStation::getLight() {
  return (_light);
}
float WeatherStation::getLightRed() {
  return (_lightRed);
}
float WeatherStation::getLightGreen() {
  return (_lightGreen);
}
float WeatherStation::getLightBlue() {
  return (_lightBlue);
}
float WeatherStation::getBatteryVoltage() {
  return (_batteryVoltage);
}
float WeatherStation::getBatteryTemp() {
  return (_batteryTemp);
}
float WeatherStation::getTempRTC() {
  return (_tempRTC);
}


void WeatherStation::setRain(float value) {
  _rain = value;
}
void WeatherStation::setRain24h(float value, int indice) {
  _rain24h[indice] = value;
}
void WeatherStation::setRain7d(float value, int indice) {
  _rain7d[indice] = value;
}
void WeatherStation::setWindDir(float value) {
  _windDir = value;
}
void WeatherStation::setWindSpeed(float value) {
  _windSpeed = value;
}
void WeatherStation::setTempDHT(float value) {
  _tempDHT = value;
}
void WeatherStation::setTempBMP(float value) {
  _tempBMP = value;
}
void WeatherStation::setHumidity(float value) {
  _humidity = value;
}
void WeatherStation::setPressure(float value) {
  _pressure = value;
}
void WeatherStation::setAltitude(float value) {
  _altitude = value;
}
void WeatherStation::setLight(float value) {
  _light = value;
}
void WeatherStation::setLightRed(float value) {
  _lightRed = value;
}
void WeatherStation::setLightGreen(float value) {
  _lightGreen = value;
}
void WeatherStation::setLightBlue(float value) {
  _lightBlue = value;
}
void WeatherStation::setBatteryVoltage(float value) {
  _batteryVoltage = value;
}
void WeatherStation::setBatteryTemp(float value) {
  _batteryTemp = value;
}
void WeatherStation::setTempRTC(float value) {
  _tempRTC = value;
}

void WeatherStation::windDirAngle2Direction(){
  // set the string value for the icon for temperature in the adafruitIO
  int angle =  _windDir;
  String iconName = "w:wind__from-";

  angle = (angle + 180) % 360;
  iconName = iconName + String(angle) + "-deg";
  _iconNameWindDir = iconName;
 }

void WeatherStation::pressure2Forecast(){
  // set the string value for the icon for pressure in the adafruitIO

	if (_pressure<1006){
		_iconPressureForecast = "w:rain";
 }
 if((_pressure < 1013) && (_pressure >= 1006)){
	 		_iconPressureForecast = "w:day-rain";
 }
 if((_pressure < 1020) && (_pressure >= 1013)) {
	 		_iconPressureForecast = "w:day-sunny-overcast";
 }
 if(_pressure >= 1020){
	 		_iconPressureForecast = "w:day-sunny";
 }
}


void WeatherStation::addRain24h(float value, int indice) {
  _rain24h[indice] += value;
}
void WeatherStation::addRain7d(float value, int indice) {
  _rain7d[indice] += value;
}
void WeatherStation::addRainGroup(DateTime instant){

uint8_t  hour = instant.hour();
uint8_t day = instant.dayOfTheWeek();

addRain24h(_rain, hour);
addRain7d(_rain, day);

}

/*******************************************************************************************************/
/* Function for radio message */
/*******************************************************************************************************/

void WeatherStation::value2Buff(float value, int start, bool tempTest)
{
  // this fonction transforme the value in int
  // then write the value in the 4 byte starting at start

  int valueInt;
  if (tempTest) {
    // if it's a temp value, add an offset of +40°C to avoid negative number then multiply by 10
    valueInt = int(roundf(((value + 40) * 10)));
  }
  else
  { // else only multiply by 10 to keep one digit
    valueInt = int(roundf(value * 10));
  }

  _radioBuffer[start + 3] = valueInt & 0xff;
  _radioBuffer[start + 2] = (valueInt >> 8) & 0xff;
  _radioBuffer[start + 1] = (valueInt >> 16) & 0xff;
  _radioBuffer[start] = (valueInt >> 24) & 0xff;
}

float WeatherStation::buff2Value(int start, bool tempTest)
{
  int valueInt;
  byte byte1 = _radioBuffer[start]; // compter de gauche à droite
  byte byte2 = _radioBuffer[start + 1];
  byte byte3 = _radioBuffer[start + 2];
  byte byte4 = _radioBuffer[start] + 3;
  valueInt = (int)(byte1 << 24 | byte2 << 16 | byte3 << 8 | byte4);

  float value;
  if (tempTest) {
    // if it's a temp value, divise by 10 then remove the +40°C offset
    value = (float(valueInt) / 10) - 40;
  }
  else
  { // else only divise by 10
    value = float(valueInt) / 10;
  }

  return (value);
}

void WeatherStation::codingMessage()
{
  // this function create the message for the radio
  value2Buff(_rain, 0);
  value2Buff(_windDir, 4);
  value2Buff(_windSpeed, 8);
  value2Buff(_tempDHT, 12, true);
  value2Buff(_humidity, 16);
  value2Buff(_tempBMP, 20, true);
  value2Buff(_pressure, 24);
  value2Buff(_tempRTC, 28, true);
  value2Buff(_light, 32);
  value2Buff(_lightRed, 36);
  value2Buff(_lightGreen, 40);
  value2Buff(_lightBlue, 44);
  value2Buff(_batteryVoltage, 48);
  value2Buff(_batteryTemp, 52, true);
}

void WeatherStation::decodingMessage()
{
  _rain = buff2Value(0);
  _windDir = buff2Value(4);
  _windSpeed = buff2Value(8);
  _tempDHT = buff2Value(12, true);
  _humidity = buff2Value(16);
  _tempBMP = buff2Value(20, true);
  _pressure = buff2Value(24);
  _tempRTC = buff2Value(28, true);
  _light = buff2Value(32);
  _lightRed = buff2Value(36);
  _lightGreen = buff2Value(40);
  _lightBlue = buff2Value(44);
  _batteryVoltage = buff2Value(48);
  _batteryTemp = buff2Value(52, true);
}

void WeatherStation::setRadioBufferReceive(uint8_t* message)
{
  // this function write the message received into the radio buffer of the class
  // after, only a call to decodingMessage will be enough

  for (int i = 0; i < 62; i++)
  {
    _radioBuffer[i] = message[i];
  }
}

uint8_t* WeatherStation::getRadioBuffer(){
  return(_radioBuffer);
}

void WeatherStation::calculateIndex(){
  // calculate all the four index of temperature

  _avgTemp = (_tempBMP + _tempDHT) / 2;
  _dewPoint = dewPoint(_avgTemp, _humidity);
  _icingPoint = icingPoint(_avgTemp, _dewPoint);
	_heatIndex = heatIndex(_avgTemp, _humidity);
  _windChill = windChill(_avgTemp, _windSpeed);
 }



/*******************************************************************************************************/
/* Weather function for temperature index */
/*******************************************************************************************************/


float degreC2F(float tempC) {
  return ((tempC * 9 / 5) + 32);
}

float degreF2C(float tempF) {
  return ((tempF - 32) * 5 / 9);
}

float dewPoint(float tempC, float humidity)
{
  // calculate the dew point using the simplified formula on :
  // https://weather.station.software/blog/what-are-dew-and-frost-points/
  if ((tempC < 60) && (humidity > 0) && (humidity < 100))
  {
    double dewPoint;
    dewPoint = pow( double(humidity / 100), double(1 / 8)) * (112 + (0.9 * double(tempC))) + (0.1 * double(tempC)) - 112;
    return (float(dewPoint));
  }
  return (-1);
}

float icingPoint(float tempC, float dewPoint)
{
  // this function calculate the icing point of water with the tempC and the dew point
  // https://weather.station.software/blog/what-are-dew-and-frost-points/
  double icingPoint;
  icingPoint = 2671.02 / ((2954.61 / (double(tempC) + 273.15)) + 2.193665 * log(double(tempC) + 273.15) - 13.3448);
  icingPoint += (double(dewPoint) + 273.15) - double(tempC + 273.15) - 273.15;
  return (float(icingPoint));
}

float windChill(float tempC, float windSpeed)
{
  // calculate the temperature as the body will feel it
  // tempC in °C
  // windSpeed in km/h
  // formula on : https://fr.wikipedia.org/wiki/Refroidissement_%C3%A9olien
  float tempWindChill;
  if (windSpeed < 4.8)
  {
    tempWindChill = tempC + 0.2 * (0.1345 * tempC - 1.59) * windSpeed;
    return (tempWindChill);
  }
  tempWindChill = 13.12 + 0.6215 * tempC + (0.3965 * tempC - 11.37) * float(pow(double(windSpeed), 0.16));
  return (tempWindChill);
}

float heatIndex(float tempC, float humidity)
{
  // calculate the heat index with the tempC and humidity
  // https://en.wikipedia.org/wiki/Heat_index

  double tempF = double(degreC2F(tempC)); //put the tempC in fahrentheit
  double heatIndex;
  double humidityDouble;

  heatIndex = -42.379;
  heatIndex += 2.0490 * tempF;
  heatIndex += 10.1433 * humidityDouble;
  heatIndex += -0.2248 * tempF * humidityDouble;
  heatIndex += -6.8378 * 0.001 * tempF * tempF;
  heatIndex += -5.4817 * 0.01 * humidityDouble * humidityDouble;
  heatIndex += 1.2287 * 0.001 * tempF * tempF * humidityDouble;
  heatIndex += 8.5282 * 0.0001 * tempF * humidityDouble * humidityDouble;
  heatIndex += -1.99 * 0.000001 * tempF * tempF * humidityDouble * humidityDouble;

  return (degreF2C(float(heatIndex))); // convert to °C
}
