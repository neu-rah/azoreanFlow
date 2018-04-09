#include "azorean.h"

static unsigned long lastFake;

unsigned long getMillis() {
  #ifdef Neurs
    return millis()<<0;
  #else
    return millis();
  #endif
}

void readTemperature() {
  #ifdef Neurs
    input=fakeTemp;
  #else
    input = _THRM1.readCelsius();
  #endif
}

void heatTo(double target,unsigned long e) {
#ifdef DEBUG
    Serial<<"Heating to "<<target;
#endif
  digitalWrite(FAN_SSR, FAN_OFF);//ensure fan off
  // unsigned long e=flow.elapsedTime(now);
  while(input<target) {
    digitalWrite(HP_SSR, HP_ON);
    now=getMillis();
    fakeHW(now-lastFake);
    lastFake=now;
    readTemperature();
    plot(input,e);
    delay(100);
  };
  digitalWrite(HP_SSR, HP_OFF);
#ifdef DEBUG
    Serial<<" (done)"<<endl;
#endif
}

void chillTo(double target,unsigned long e) {
#ifdef DEBUG
    Serial<<"Chilling to room "<<target;
#endif
  digitalWrite(HP_SSR, HP_OFF);//ensure heater is off!
  // unsigned long e=flow.elapsedTime(now);
  while(input>TEMPERATURE_ROOM) {
    digitalWrite(FAN_SSR, FAN_ON);
    now=getMillis();
    fakeHW(now-lastFake);
    lastFake=now;
    readTemperature();
    plot(input,e);
    delay(100);
  };
  digitalWrite(FAN_SSR, FAN_OFF);
#ifdef DEBUG
    Serial<<" (done)"<<endl;
#endif
}

void gotoTemp(double target,unsigned long e) {
  readTemperature();
#ifdef DEBUG
    Serial<<"read temp:"<<input<<endl;
#endif
  if (input>target) chillTo(target,e);
  else if (input<target) heatTo(target,e);
}

template<unsigned long period>
inline bool blink(double power,unsigned long now) {return power<(now%period);}

void reflow() {
    // REFLOW //////////////////////////////////////////////////////////////////
    _TFT.clearScreen();
#ifdef DEBUG
    Serial<<"Reflow start ---------------------"<<endl;
#endif
    //plot curve preview
    plotPreview<5>(flow);

    now = getMillis();
    lastFake=now;
    fakeHW(1);
    gotoTemp(TEMPERATURE_ROOM,0);

    //setup PID's
    _RFLWPID.SetOutputLimits(0, hpPeriod/cycleTime);
    _FANPID.SetOutputLimits(0, fanPeriod);
    _RFLWPID.SetSampleTime(PID_SAMPLE_TIME);
    _FANPID.SetSampleTime(PID_SAMPLE_TIME);
    // Turn the PID on
    _RFLWPID.SetMode(AUTOMATIC);
    _FANPID.SetMode(AUTOMATIC);

    now = getMillis();
    // nextRead = now;

    flow.start(input,now);
    bool processing=true;
    do {//flow process till it ends ==========================================
        switch(_ENC.getButton()) {
            case ClickEncoder::Held:
                run=false; //abort!
#ifdef DEBUG
                Serial<<"Aborting..."<<endl;
#endif
                tone(BEEPER,BEEP_FREQ,1200);
                break;
            case ClickEncoder::DoubleClicked:
                digitalWrite(FAN_SSR,!digitalRead(FAN_SSR));
#ifdef DEBUG
                Serial<<"Fan : "<<(digitalRead(FAN_SSR)?"OFF":"ON")<<endl;
#endif
                tone(BEEPER,BEEP_FREQ,500);
                break;
        }
        now=getMillis();
        if (now>lastFake) {
          fakeHW(now-lastFake);
          lastFake=now;
        }
        unsigned long flowTime=flow.elapsedTime(now);
        //PID will only update on specified period (internally)
        //so we need not to reched it externally
        readTemperature();
        setpoint=flow.getPoint();

        plot(input,flowTime);
        processing=flow.process(input,now);
        if (hpOnCool||flow.heating())
            _RFLWPID.Compute();
        if (fanOnHeat||flow.cooling())
            _FANPID.Compute();
        if (flow.heating()) {
          digitalWrite(HP_SSR,blink<hpPeriod>(output*cycleTime,flowTime)?HP_OFF:HP_ON);
          if (fanOnHeat) digitalWrite(FAN_SSR,blink<fanPeriod>(fanOut,flowTime)?FAN_OFF:FAN_ON);
          else digitalWrite(FAN_SSR,FAN_OFF);
        }
        if (flow.cooling()) {
          digitalWrite(FAN_SSR,blink<fanPeriod>(fanOut,flowTime)?FAN_OFF:FAN_ON);
          if (hpOnCool) digitalWrite(HP_SSR,blink<hpPeriod>(output*cycleTime,flowTime)?HP_OFF:HP_ON);
          else digitalWrite(HP_SSR,HP_OFF);
        }
  } while (run&&processing);
#ifdef DEBUG
    Serial<<(run?"Terminated.":"Interrupted!")<<endl;
#endif
    _RFLWPID.SetMode(MANUAL);
    _FANPID.SetMode(MANUAL);
    gotoTemp(TEMPERATURE_ROOM,flow.elapsedTime(now));
    run=false;
    
    delay(300);
    tone(BEEPER,BEEP_FREQ,600);
    delay(300);
    tone(BEEPER,BEEP_FREQ,600);
}

void splash() {
    _TFT.clearScreen();
    _TFT.setFont(ucg_font_helvB08_tf); // ucg_font_profont11_mr);
    _TFT.setPrintPos(22,56);
    _TFT.print(F(NAME));
    _TFT.print(" ");
    _TFT.print(F(VERSION));
    _TFT.setPrintPos(22,80);
    _TFT.print(F("Press to start reflow"));
#ifdef DEBUG
    Serial<<NAME<<" "<<VERSION<<endl;
#endif
    //now it stays there till clicked
    /*#ifdef Neurs
        delay(500);
    #else
        delay(3500);
    #endif*/
}
