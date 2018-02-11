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

#include <PDQ_GFX.h>             // PDQ: Core graphics library
#include "PDQ_ST7735_config.h"   // PDQ: ST7735 pins and other setup for this sketch
#include <PDQ_ST7735.h>          // PDQ: Hardware-specific driver library


// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   14	// SW SPI only
#define MAXCLK  15	// SW SPI Only
#define MAXCS   3	// Sensor Chip Select

// TFT
static const uint8_t PIN_LCD_CS   = 10;
static const uint8_t PIN_LCD_DC   = 9;
static const uint8_t PIN_LCD_RST  = 8;
static const uint8_t LCD_ROTATION = 3; // 0/2-> portrait, 1/3-> landscape

PDQ_ST7735 tft;     // PDQ: create LCD object (using pins in "PDQ_ST7735_config.h")

// initialize the Thermocouple instance with hardware SPI on a given CS pin or in SW SPI mode
HP_MAX31855 thermocouple(MAXCS); // HW SPI
//HP_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO); // SW SPI


void setupTFT() {
  FastPin<ST7735_RST_PIN>::setOutput();
  FastPin<ST7735_RST_PIN>::hi();
  FastPin<ST7735_RST_PIN>::lo();
  delay(1);
  FastPin<ST7735_RST_PIN>::hi();
  
  tft.begin();
  
  tft.setTextWrap(false);
  tft.setTextSize(1);
  tft.setRotation(LCD_ROTATION);
  tft.fillScreen(ST7735_BLACK);
  delay(15);
  tft.fillScreen(ST7735_WHITE);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
}

void displaySplash() {
  tft.fillScreen(ST7735_WHITE);     
  tft.setTextColor(ST7735_BLACK);
  // splash screen
  tft.setCursor(2, 30);
  tft.setTextSize(2);
  tft.print("Thermocouple");
  tft.setCursor(tft.width()-120, 48);
  tft.print("Controller");
  tft.setTextSize(1);
  tft.setCursor(52, 67);
  tft.print("v1.0");
  delay(2000);
}

void setup() {
  pinMode(MAXCS, OUTPUT);
  pinMode(PIN_LCD_CS, OUTPUT);

  setupTFT();

  thermocouple.setVerbose(false); // output values on serial
  // Set Serial if needed
  if (thermocouple.getVerbose())
  {
    Serial.begin(115200);
    while (!Serial) {}; // wait for Serial on Leonardo/Zero, etc
    Serial.println("MAX31855 on TFT print");  
  }
  
  // wait for MAX chip to stabilize - min 500ms
  displaySplash();

  tft.fillScreen(ST7735_BLACK);     
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.setCursor(4,15);
  tft.setTextSize(2);
  tft.print("TEMP Reading");
}

void loop() {
  // basic readout test, just print the current temp
  volatile float c = 0.0;
  volatile float f = 0.0;
  volatile float pc = 1.0;
  volatile float pf = 1.0;
  thermocouple.readInternal();      //  RAW value
  c = thermocouple.readCelsius();   // display ºC only
  f = thermocouple.readFarenheit(); // display ºC and ºF

  if ( (c != pc) || (f != pf) )
  {
    // To display
    tft.setTextSize(1);
    tft.setCursor(4, 35);
    tft.print(c);
    tft.print("\367C  ");
    tft.print(f);
    tft.println("\367F");

    pc = c;
    pf = f;
  }
  delay(1000);
}
