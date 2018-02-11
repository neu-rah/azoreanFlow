/* -*- C++ -*- */


#ifndef HP_REFLOW_BASE_H
#define HP_REFLOW_BASE_H

  #include "config.h"
  #include "flow.h"
  #include "profiles.h"

  #include "fake.h"
  // #include "plot.h"

  // ***** VARS *****
  extern bool run;
  extern unsigned long now;
  constexpr bool fanOnHeat=false;
  constexpr bool hpOnCool=true;
  constexpr uint16_t BEEP_FREQ = 1976; // B6 note
  // extern unsigned long nextRead;
  enum FlowError:int {
    FlowOk=0,
    MaxTempReached,
    Lagging
  };
  constexpr double maxTemp=230;

  // ***** PID CONTROL VARIABLES *****
  extern double setpoint;//target temperature (guiding PID)
  extern double input;//current temperature (read by PID)
  extern double output;//heat power (set by PID)
  extern double fanOut;//fan power (set by PID)

  // constexpr int windowSize=2000;//power range used by PID
  constexpr int gridFreq=50;//Hz
  constexpr int hpPeriod=2000;
  constexpr int fanPeriod=2000;
  constexpr int cycleTime=1000/gridFreq;
  // extern unsigned long windowStartTime;//to control elements on/off inside window

  // Specify PID control interface
  extern PID _RFLWPID;
  extern PID _FANPID;

  // Specify MAX31855 _THRM1 interface
  extern HP_MAX31855 _THRM1;
  extern HP_MAX31855 _THRM2;

  // HW SPI TFT DISPLAY
  extern Ucglib_ST7735_18x128x160_HWSPI _TFT;

  // Rotary Encoder
  extern ClickEncoder _ENC;

  extern double fakeTemp;

#endif
