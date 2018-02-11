/*************************************************** 
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include <HP_MAX31855.h>


// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   14
#define MAXCLK  15
#define MAXCS   3  // Sensor 

// initialize the Thermocouple
//HP_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);  // SW SPI
HP_MAX31855 thermocouple(MAXCS); // HW SPI

void setup() {
  Serial.begin(115200);
 
  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc

  Serial.println("MAX31855 Serial print");
  thermocouple.setVerbose(true); // output values on serial
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  thermocouple.readInternal();  // display RAW value
  //thermocouple.readCelsius(); // display ºC only
  thermocouple.readFarenheit(); // display ºC and ºF

   delay(1000);
}
