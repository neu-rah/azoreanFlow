#include "base.h"

// ***** VARS *****
bool run=false;
unsigned long now;
// unsigned long nextRead;

// ***** PID CONTROL VARIABLES *****
double setpoint;//target temperature (guiding PID)
double input;//current temperature (read by PID)
double output;//heat power (set by PID)
double fanOut;//fan power (set by PID)
// unsigned long windowStartTime;//to control elements on/off inside window

PID _RFLWPID(&input, &output, &setpoint, PID_KP, PID_KI, PID_KD, DIRECT);
PID _FANPID(&input, &fanOut, &setpoint, PID_KP, PID_KI, PID_KD, REVERSE);

// Specify MAX31855 _THRM1 interface
HP_MAX31855 _THRM1(TH1_CS);
HP_MAX31855 _THRM2(TH2_CS);

// HW SPI TFT DISPLAY
Ucglib_ST7735_18x128x160_HWSPI _TFT(TFT_DC, TFT_CS, TFT_RST);

// Rotary Encoder
ClickEncoder _ENC(A_ENC, B_ENC, SW_ENC, ENC_STEPS_PER_NOTCH,LOW);

double fakeTemp=TEMPERATURE_ROOM/2;
