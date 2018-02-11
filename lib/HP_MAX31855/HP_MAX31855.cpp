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

#include "HP_MAX31855.h"
#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif

#include <stdlib.h>
#include <SPI.h>

SPISettings MAX31855SPI(4000000, MSBFIRST, SPI_MODE0);

// SW SPI Constructor
HP_MAX31855::HP_MAX31855(int8_t _sclk, int8_t _cs, int8_t _miso)
{
	sclk = _sclk;
	cs = _cs;
	miso = _miso;

	initialized = false;
}

// HW SPI Constructor
HP_MAX31855::HP_MAX31855(int8_t _cs)
{
	cs = _cs;
	sclk = miso = -1;

	initialized = false;
}


void HP_MAX31855::begin(void)
{
	//define pin modes
	pinMode(cs, OUTPUT);
	digitalWrite(cs, HIGH);

	if (sclk == -1)
		SPI.begin();
	else
	{
		pinMode(sclk, OUTPUT);
		pinMode(miso, INPUT);
	}
	initialized = true;
}


void HP_MAX31855::setVerbose(boolean _printout)
{
	printout = _printout;
}


boolean HP_MAX31855::getVerbose(void)
{
	return printout;
}



double HP_MAX31855::readInternal(void)
{
	int32_t v;
	v = spiread32();

	// ignore bottom 4 bits - they're just thermocouple data
	v >>= 4;

	// pull the bottom 11 bits off
	float internal = v & 0x7FF;
	// check sign bit!
	if (v & 0x800)
	{
		// Convert to negative value by extending sign and casting to signed type.
		int16_t tmp = 0xF800 | (v & 0x7FF);
		internal = tmp;
	}
	internal *= 0.0625; // LSB = 0.0625 degrees
	//Serial.print("\tInternal Temp: "); Serial.println(internal);
	return internal;
}


double HP_MAX31855::readCelsius(void)
{
	int32_t v;
	v = spiread32();

	//Serial.print("0x"); Serial.println(v, HEX);

	/*
	float internal = (v >> 4) & 0x7FF;
	internal *= 0.0625;
	if ((v >> 4) & 0x800) 
	internal *= -1;
	Serial.print("\tInternal Temp: "); Serial.println(internal);
	*/

	if (v & 0x7) {
		// error, check error codes on datasheet
		err = v & 0x7;
		getError();
		return NAN;
	}

	if (v & 0x80000000)
	{
		// Negative value, drop the lower 18 bits and explicitly extend sign bits.
		v = 0xFFFFC000 | ((v >> 18) & 0x00003FFFF);
	} else {
		// Positive value, just drop the lower 18 bits.
		v >>= 18;
	}
	double centigrade = v;

	// LSB = 0.25 degrees C
	centigrade *= 0.25;
	return centigrade;
}



uint8_t HP_MAX31855::getError()
{
	if (printout)
	{
		switch (err)
		{
			case FAULT_OPEN:
				Serial.println("TC ERR OPEN Circuit");
				break;
			case FAULT_SHORT_GND:
				Serial.println("TC ERR GND Short");
				break;
			case FAULT_SHORT_VCC:
				Serial.println("TC ERR VCC Short");
				break;
			default:
				Serial.println("Unknown TC ERR");
				break;
		}
	}
	return err;
}



double HP_MAX31855::readFarenheit(void)
{
	float f = (readCelsius() * (9.0/5.0)) + 32.0;
/*
	float f = readCelsius();
	f *= 9.0;
	f /= 5.0;
	f += 32;
*/
	return f;
}

// Privates

uint32_t HP_MAX31855::spiread32(void)
{
	int i;
	int32_t d = 0;

	// backcompatibility!
	if (! initialized)
		begin();

	digitalWrite(cs, LOW);
	delay(1);

	if(sclk == -1)
	{
		// hardware SPI
		SPI.beginTransaction(MAX31855SPI);
		d = SPI.transfer(0);
		d <<= 8;
		d |= SPI.transfer(0);
		d <<= 8;
		d |= SPI.transfer(0);
		d <<= 8;
		d |= SPI.transfer(0);
		SPI.endTransaction();
	} else {
		// software SPI
		digitalWrite(sclk, LOW);
		delay(1);

		for (i=31; i>=0; i--)
		{
			digitalWrite(sclk, LOW);
			delay(1);
			d <<= 1;
			if (digitalRead(miso))
				d |= 1;

			digitalWrite(sclk, HIGH);
			delay(1);
		}
	}

	digitalWrite(cs, HIGH);
	if (printout) BinaryStrZeroPad(d, 32);
	return d;
}


void HP_MAX31855::BinaryStrZeroPad(int32_t _number, signed char _zeroPadding)
{
	//ZeroPadding = nth bit, e.g for a 16 bit number nth bit = 15
	while (_zeroPadding >= 0)
	{
		//if((_number & (1<<_zeroPadding)) > 0) Serial.write('1');
		if ((_number & ( 1<<_zeroPadding )) > 0)
			Serial.write('1');
		else
			Serial.write('0');
		--_zeroPadding;
	}
	
	Serial.println();
}


