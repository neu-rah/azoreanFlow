/*******************************************************************************
 * Title: Hot Plate Reflow Controller
 * Version: 1.0.0
 * Date: 19-01-2018
 * Company: Christophe Persoz
 * Author: Christophe Persoz and Rui Avezado
 * Website: www.christophepersoz.com
 *
 * Brief
 * =====
 * This is an example firmware for our Arduino compatible reflow hot plate controller.
 * The reflow curve used in this firmware is meant for lead-free profile
 * (it's even easier for leaded process!). You'll need to use a special version of
 * the MAX31855 library for Arduino which can be downloaded from my GitHub repository.
 * Please check our wikI for more information on using this piece of code
 * together with the hot plate reflow controller shield.
 *
 * Disclaimer
 * ==========
 * Dealing with high voltage is a very dangerous act! Please make sure you know
 * what you are dealing with and have proper knowledge before hand. Your use of
 * any information or materials on this reflow oven controller is entirely at
 * your own risk, for which we shall not be liable.
 *
 * Licences
 * ========
 * This reflow oven controller hardware and firmware are released under the
 * Creative Commons Share Alike v3.0 license
 * http://creativecommons.org/licenses/by-sa/3.0/
 * You are free to take this piece of code, use it and modify it.
 * All we ask is attribution including the supporting libraries used in this
 * firmware.
 *
 * Required Libraries
 * ==================
 * - Arduino PID Library:
 *   >> https://github.com/br3ttb/Arduino-PID-Library
 *
 * Revision  Description
 * ========  ===========

 1.00		Initial release.

 *******************************************************************************/
// Board using MAX31855KASA+ chip

#include <TimerOne.h>
#include "src/azorean.h"

void timerIsr() {_ENC.service();}

void setup()
{
#ifdef DEBUG
    Serial.begin(115200);
    while (!Serial) {}
#endif
    
    // SSR pin initialization to ensure reflow plate is off
    pinMode(HP_SSR, OUTPUT);
    digitalWrite(HP_SSR, HP_OFF);
    pinMode(FAN_SSR, OUTPUT);
    digitalWrite(FAN_SSR, FAN_OFF);

    //clickEncoder timer
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerIsr);
    _ENC.setAccelerationEnabled(false);
    _ENC.setDoubleClickEnabled(true);

    // Buzzer pin initialization to ensure annoying buzzer is off
    pinMode(BEEPER, OUTPUT);
    digitalWrite(BEEPER, LOW);

    // Zero Crossing
    // pinMode(ZERO_X, INPUT_PULLUP);

    // Start-up splash + beep
    _TFT.begin(UCG_FONT_MODE_SOLID);
    #ifdef Neurs
        _TFT.setRotate90();
    #else
        _TFT.setRotate270();
    #endif

    // Thermocouple
    _THRM1.setVerbose(TH_Verb);
    _THRM2.setVerbose(TH_Verb);

    // Shortly turn on SSR
    digitalWrite(HP_SSR, HP_ON);
    digitalWrite(FAN_SSR, FAN_ON);
    tone(BEEPER,BEEP_FREQ,400);

    // Turn off SSR
    digitalWrite(HP_SSR, HP_OFF);
    digitalWrite(FAN_SSR, FAN_OFF);

    splash();
#ifdef DEBUG
    Serial<<"Click to start reflow..."<<endl;
#endif
}

//do a reflow cycle
void loop(){
    switch(_ENC.getButton()) {
        case ClickEncoder::Clicked:
            run=true;
            reflow();
#ifdef DEBUG
            Serial<<"Click to continue..."<<endl;
#endif
            while(_ENC.getButton()!=ClickEncoder::Clicked);//wait for click at end
            splash();
            break;
        case ClickEncoder::DoubleClicked://editing?
            digitalWrite(FAN_SSR,!digitalRead(FAN_SSR));
        default:break;
    }
}
