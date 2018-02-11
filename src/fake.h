/* -*- C++ -*- */

#ifndef HP_REFLOW_FAKE_H
#define HP_REFLOW_FAKE_H

  void fake_bounds();

  void fakeHW(double dt);

  constexpr double FAKE_CONST=0.01;//natural chill
  constexpr double FAKE_HEATER_FACT=0.92;//heat rate ºC/s
  constexpr double FAKE_FAN_FACT=0.41;//cool rate

#endif
