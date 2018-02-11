/*************************************************** 
  This is a library for the Adafruit Thermocouple Sensor w/MAX31855K

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

#ifndef _HP_MAX31855_H_
#define _HP_MAX31855_H_

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define FAULT_OPEN		0x1
#define FAULT_SHORT_GND 0x2
#define FAULT_SHORT_VCC 0x4

class HP_MAX31855
{
	public:
		HP_MAX31855(int8_t _sclk, int8_t _cs, int8_t _miso);
		HP_MAX31855(int8_t _cs);

		void begin(void);
		double readInternal(void);
		double readCelsius(void);
		double readFarenheit(void);
		void setVerbose(boolean _printout);
		boolean getVerbose();
		uint8_t getError();

	private:
		boolean initialized;
		boolean printout;

		int8_t sclk, miso, cs;
		uint8_t err;
		uint32_t spiread32(void);
		void BinaryStrZeroPad(int32_t _number, signed char _zeroPadding);
};

#endif
