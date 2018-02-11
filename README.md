# Azorean Hot Plate Reflow Soldering
## Project
HP_ReflowSoldering  
Hardware by Christophe Persoz on 19/01/2018  
Software by Rui Avezedo  
Licence GNU General Public License v3 


## References

## Info
#### This is an Arduino-based reflow plate controller with
* PID loop control  
* Wave Packet and Phase Fired control for AC outputs  
* Use Zero Crossing SSR
* Graphic TFT LC-Display, drawing the temperature curves using a 1.8" TFT or derivate display
* ~~Controlled using a cheap rotary encoder and its~~ single button  
* ~~Stores up to 30 temperature profiles in EEPROM~~  (not yet)
* Configurable PID-parameters  
* Simple, small hardware to drive loads up to 400V and up to 6A  
* Hardware can measure two temperatures independently drive one AC load and one DC load, such as a fan

## Main Characteristics
* 2 K-thermocouple input associated to MAX31588 HE SPI controller  
* 2 SSR output (One for Heat Plate - 6A, the other for a 12V Fan - 1A)  
* ~~Rotary encoder with switch for user input~~
* TFT 1.8" HW SPI Color display for feedback and control  
* Beeper for advertising  

~~Please Note: Requires Arduino IDE 1.5.x or newer.~~
Based on 0xPIT hardware, but contains a lot of updates on hardware and a complete new software.
