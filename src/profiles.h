/* -*- C++ -*- */
/*
*
* Temperature (Degree Celcius)                 Magic Happens Here!
* 245-|                                               x  x
*     |                                            x        x
*     |                                         x              x
*     |                                      x                    x
* 200-|                                   x                          x
*     |                              x    |                          |   x
*     |                         x         |                          |       x
*     |                    x              |                          |
* 150-|               x                   |                          |
*     |             x |                   |                          |
*     |           x   |                   |                          |
*     |         x     |                   |                          |
*     |       x       |                   |                          |
*     |     x         |                   |                          |
*     |   x           |                   |                          |
* 30 -| x             |                   |                          |
*     |<  60 - 90 s  >|<    90 - 120 s   >|<       90 - 120 s       >|
*     | Preheat Stage |   SoakIng Stage   |       Reflow Stage       | Cool
*  0  |_ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*                                                                Time (Seconds)
*
*/
#include "flow.h"

#ifndef HP_REFLOW_PROFILES_H
#define HP_REFLOW_PROFILES_H

  namespace Sn965_Ag30_Cu05 {
    extern FlowSegment* segments[];
    extern AzoreanFlow<5> flow;
  };//Sn965_Ag30_Cu05

  namespace SnDEBUG {
    extern FlowSegment* segments[];
    extern AzoreanFlow<5> flow;
  };//SnDEBUG

  namespace Sn965_Ag30_Cu05_smooth {
    extern FlowSegment* segments[];
    extern AzoreanFlow<5> flow;
  };

#endif
