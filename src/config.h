/* -*- C++ -*- */
// ***** INCLUDES *****
#ifndef HP_REFLOW_CONFIG_H
#define HP_REFLOW_CONFIG_H

  //#ifdef DEBUG
    #include <streamFlow.h>
  //#endif
  #include <HP_MAX31855.h>			// Thermocouple HW SPI
  #include <PID_v1.h>				// PID library
  #include <ClickEncoder.h>			// Rotary encoder + switch
  #include <Ucglib.h>				// HW SPI TFT Graphic display
  // #include "flow.h"

  // ***** CONSTANTS *****
    #define VERSION "1.01"
    #define NAME    "azoreanREFLOW"

  #define FAN_ON LOW
  #define FAN_OFF HIGH
  #define HP_ON LOW
  #define HP_OFF HIGH

  #define TEMPERATURE_ROOM 33
  //#define TEMP_COMPENSATION 0

  constexpr unsigned long PID_SAMPLE_TIME=1000;
  constexpr unsigned long COMPENSATION=PID_SAMPLE_TIME;

  enum DriveMode {TempDrived,TimeDrived};
  constexpr DriveMode driveMode=TimeDrived;

  #define PID_KP				300 //1.75 // 300
  #define PID_KI				0.05 //0.03 // 0.05
  #define PID_KD				250 //3.0  // 250

  #define tempColor             174,255,0
  #define baseColor             235,235,235
  #define auxColor              63,63,63
  #define hiliteColor           255,161,0
  #define selColor              tempColor

  #ifdef Neurs
    // ***** PIN ASSIGNMENT in Azores ;) *****
    constexpr uint8_t HP_SSR  = 17;//5;	// Pin for SSR1
    constexpr uint8_t FAN_SSR = 19;	// Pin for SSR2

    constexpr uint8_t TH1_CS  = A6;	// Pin for Thermocouple 1
    constexpr uint8_t TH2_CS  = A7;	// Pin for Thermocouple 2
    constexpr bool TH_Verb = false;

    constexpr uint8_t ZERO_X  = 18;	// Pin for zero crossing detector
    constexpr uint8_t BEEPER  = 6;	// Beeper

    constexpr uint8_t SW_ENC  = 4;	// Rotary Encoder Switch
    constexpr uint8_t A_ENC   = 2;	// Rotary Encoder A
    constexpr uint8_t B_ENC   = 3;	// Rotary Encoder B

    constexpr uint8_t ENC_STEPS_PER_NOTCH = 4; // Encoder Step Notch
    constexpr uint8_t TFT_CS   = A1;
    constexpr uint8_t TFT_DC   = A0;
    constexpr uint8_t TFT_RST  = A2;
  #else
    //this ignores fakeHW calls
    #define fakeHW(x)

    // ***** PIN ASSIGNMENT *****
    constexpr uint8_t HP_SSR  = 0;	// Pin for SSR1
    constexpr uint8_t FAN_SSR = 1;	// Pin for SSR2
    constexpr uint8_t TH1_CS  = 3;	// Pin for Thermocouple 1
    constexpr uint8_t TH2_CS  = 4;	// Pin for Thermocouple 2
    constexpr bool    TH_Verb = false;
    constexpr uint8_t ZERO_X  = 2;	// Pin for zero crossing detector
    constexpr uint8_t BEEPER  = 5;	// Beeper
    constexpr uint8_t SW_ENC  = A0;	// Rotary Encoder Switch
    constexpr uint8_t A_ENC   = A1;	// Rotary Encoder A
    constexpr uint8_t B_ENC   = A2;	// Rotary Encoder B
    constexpr uint8_t ENC_STEPS_PER_NOTCH = 4; // Encoder Step Notch
    constexpr uint8_t TFT_CS   = 10;
    constexpr uint8_t TFT_DC   = 9;
    constexpr uint8_t TFT_RST  = 8;

  #endif

  #if defined(DEBUG) && defined(TRACE)
    #define trace(x) x
  #else
    #define trace(x)
  #endif
  #ifdef DEBUG
    #define _trace(x) x
  #else
    #define _trace(x)
  #endif

#endif
