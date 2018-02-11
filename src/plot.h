/* -*- C++ -*- */
#ifndef HP_REFLOW_PLOT_H
#define HP_REFLOW_PLOT_H

  #include "base.h"

  // plot scale
  constexpr float PLOT_FACTOR_X=0.35;
  constexpr float PLOT_FACTOR_Y=64.0;

  enum PlotType {plotSlope,plotVert,plotHoriz};

  void plot(double input,unsigned long t);
  unsigned long plotSegment(FlowSegment& segment,double entry,unsigned long at,PlotType type);

  template<uint8_t nSeg>
  void _plotPreview(AzoreanFlow<nSeg> flow,PlotType type) {
    unsigned long at=0;
    int e=TEMPERATURE_ROOM;
    for(int n=0;n<nSeg;n++) {
      FlowSegment* s=flow.segments[n];
      at+=plotSegment(*s,e,at,type);
      e=s->target;
    }
  }

  template<uint8_t nSeg>
  void plotPreview(AzoreanFlow<nSeg> flow) {
    _TFT.setColor(auxColor);
    switch(driveMode) {
      case TimeDrived: _plotPreview<nSeg>(flow,plotVert);break;
      case TempDrived: _plotPreview<nSeg>(flow,plotHoriz);break;
    }
    _TFT.setColor(baseColor);
    _plotPreview<nSeg>(flow,plotSlope);
    // _TFT.setColor(255,255,255);
  }
#endif
