#include "plot.h"

inline void fieldColor(bool sel=false) {
  sel?_TFT.setColor(baseColor):_TFT.setColor(selColor);
}

void plot(double input,unsigned long t) {
  int s=t/1000;
  int m=s/60;
  s%=60;
  // Print current temperature
  //_TFT.setFontMode(UCG_FONT_MODE_SOLID);
  _TFT.setPrintPos(2,10);
  fieldColor();
  _TFT.print(F("Time "));
  _TFT.setColor(hiliteColor);
  _TFT.print(m);
  _TFT.print(":");
  if (s<10) _TFT.print("0");
  _TFT.print(s);
  fieldColor();
  _TFT.setPrintPos(100,10);
  _TFT.print(F("Th1 "));
  _TFT.setColor(hiliteColor);
  _TFT.print(input);
  // Print degree Celsius symbol
  _TFT.print(F("\260"));

    _TFT.setPrintPos(100,26);
    fieldColor();
    _TFT.print(F("Th2 "));
    _TFT.setColor(hiliteColor);
    _TFT.print(_THRM2.readCelsius());
    // Print degree Celsius symbol
    _TFT.print(F("\260"));
    _TFT.setColor(baseColor);

  _TFT.setColor(baseColor);

  // Serial<<" HP Kp:"<<_RFLWPID.GetKp()<<" Ki:"<<_RFLWPID.GetKi()<<" Kd:"<<_RFLWPID.GetKd()<<endl;
  // Serial<<"Fan Kp:"<<_FANPID.GetKp()<<" Ki:"<<_FANPID.GetKi()<<" Kd:"<<_FANPID.GetKd()<<endl;

  // PLOTTING
  uint8_t x=t/1000*PLOT_FACTOR_X;

  #ifdef DEBUG
    //plot setpoint (PID target)
    _TFT.setColor(255,0,0);
    _TFT.drawPixel(x,120.0-setpoint/250.0*PLOT_FACTOR_Y);
  #endif
  #ifdef Neurs
    //plot fan
    _TFT.setColor(0,0,255);
    _TFT.drawPixel(x,120.0-fanOut/fanPeriod*PLOT_FACTOR_Y);

    //plot heater power
    _TFT.setColor(255,255,0);
    _TFT.drawPixel(x,120.0-output/hpPeriod*PLOT_FACTOR_Y);
  #endif

  //plot temperature
  _TFT.setColor(tempColor);
  _TFT.drawPixel(x,120.0-input/250.0*PLOT_FACTOR_Y);

  //set color back to white
  _TFT.setColor(255,255,255);
}

unsigned long plotSegment(FlowSegment& segment,double entry,unsigned long at,PlotType type) {
  uint8_t x=at/1000*PLOT_FACTOR_X;
  unsigned long l=segment.segTime(entry);
  trace(Serial<<"segTime:"<<l<<endl);
  uint8_t _x=x+l/1000*PLOT_FACTOR_X;
  uint8_t y=120.0-entry/250*PLOT_FACTOR_Y;
  uint8_t _y=120.0-segment.target/250*PLOT_FACTOR_Y;
  switch(type) {
    case plotSlope:
      _TFT.drawLine(
        x,y,
        _x,_y
      );
      break;
    case plotVert: _TFT.drawVLine(_x, _y, 120-_y);break;
    case plotHoriz: _TFT.drawLine(0,_y,160,_y);break;
  }
  return l;
}
