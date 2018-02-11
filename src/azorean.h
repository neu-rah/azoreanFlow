/* -*- C++ -*- */
#ifndef HP_REFLOW_PASSIVE_H
#define HP_REFLOW_PASSIVE_H

#include "base.h"
#include "plot.h"
#ifdef DEBUG
    // using namespace SnDEBUG;
    // using namespace Sn965_Ag30_Cu05;
    using namespace Sn965_Ag30_Cu05_smooth;
#else
    // using namespace Sn965_Ag30_Cu05;
    using namespace Sn965_Ag30_Cu05_smooth;
#endif

void readTemperature();
void gotoTemp(double target,unsigned long e);
void reflow();
void splash();
#endif
