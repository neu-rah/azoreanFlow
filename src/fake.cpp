#include "base.h"

#ifdef Neurs
void fake_bounds() {
  if (fakeTemp<TEMPERATURE_ROOM/2)//lower bound
    fakeTemp=TEMPERATURE_ROOM/2;
  if (fakeTemp>300) {
#ifdef DEBUG
      Serial<<"ALERT TEMP:"<<fakeTemp<<endl;
#endif
    delay(1000);
    fakeTemp=300;//upper bound
  }
}

void fakeHW(double dt) {
    dt/=1000;//in seconds
    // long ms=millis();
    // static unsigned long lastFake=ms;
    fake_bounds();
    // Serial<<"faking "<<fakeTemp<<" ";
    double unit=FAKE_CONST*dt;
    // Serial<<unit;
    fakeTemp-=unit;//natural chilling
    double h=0;
    double f=0;
    if (digitalRead(HP_SSR)==HP_ON) h=FAKE_HEATER_FACT*dt;//heating
    if (digitalRead(FAN_SSR)==FAN_ON) f=FAKE_FAN_FACT*dt;//fan chilling
    fakeTemp+=h-f;
    // Serial<<" +"<<h<<"-"<<f;
    fake_bounds();
    // Serial<<" "<<fakeTemp<<"ºC"<<endl;;
    // lastFake=ms;
}
#endif
