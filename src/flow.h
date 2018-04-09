/* -*- C++ -*- */
#ifndef HP_REFLOW_FLOW_H
#define HP_REFLOW_FLOW_H

  #include "config.h"

  //concatenable reflow segment
  // storing a slope and a target temperature
  //provides preview for any part of the segment (random-access)
  //checks reached and rested at target
  class FlowSegment {
  public:
    // FlowSegment(const char*name,double slope,double target,unsigned long duration=0)
    //   :name(name),slope(slope/1000.0),target(target),duration(duration) {}
    FlowSegment(const char*name,double start,double end,unsigned long time, unsigned long duration=0)
      :name(name),slope((end-start)/time/1000),target(end),duration(duration) {}
    // FlowSegment(double target):name("waiting..."),target(target) {}
    // FlowSegment(unsigned long duration):name("waiting..."),duration(duration) {}

    //get total time to execute (ms)
    inline unsigned long segTime(double entry) const {
      unsigned long r=(target-entry)/slope;
      // Serial<<"segTime entry:"<<entry<<" target:"<<target<<" slope:"<<slope*1000<<" = "<<r<<endl;
      return r;
    }
    //get direction heat/cold
    inline bool heating() const {return slope>0;}
    inline bool cooling() const {return slope<0;}
    //segment reached the target temperature?
    inline bool reached(double temp) const {
      return (slope>0&&temp>=target)||(slope<0&&temp<=target)||temp==target;
    }
    //rested duration at target temperature?
    inline bool rested(unsigned long t) const {return t>=duration;}

    //get (relative) temperature at any point of the segment
    inline double getPoint(unsigned long t) const {return t*slope;}
    const char* name;//segment name
    double slope=0;// ºC/ms
    double target=0;// target temperature ºC
    unsigned long duration=0;//remain on target for this duration (ms)
  };

  // process a sequence of reflow segments
  // call start and then process till it returns false
  template<uint8_t nSeg>
  class AzoreanFlow {
  public:
    AzoreanFlow(double startPoint,FlowSegment* flow[],unsigned long updateTime)
      :startPoint(startPoint),segments(flow),updateTime(updateTime) {}
    //size of sequence (# of segments)
    static inline uint8_t sz() {return nSeg;}
    //current segment
    inline uint8_t at() {return idx;}
    inline bool heating() const {return segments[idx]->heating();}
    inline bool cooling() const {return segments[idx]->cooling();}
    inline unsigned long elapsedTime(unsigned long t) {return t-started;}
    void start(double temp,unsigned long t=0) {
      // if(temp>TEMPERATURE_ROOM+5) return false;
      //TODO: check if target is in the slope diretion!
#ifdef DEBUG
        Serial<<"Sequence start"<<endl;
#endif
      started=t;
      segmentStarted=t;
      idx=0;
      onTarget=false;
      entryPoint=temp;
#ifdef DEBUG
        report();
#endif
    }
    FlowSegment** segments;
    inline bool process(double temp,unsigned long t) {
      // #ifdef Neurs
      switch (driveMode) {
        case TimeDrived:return idealProfile(t+COMPENSATION);
        case TempDrived:return _process(temp,t);
      }
    }
    //ideal idealProfile atempt
    //TODO: remove `started` make this time relative!
    bool idealProfile(unsigned long t) {
      t-=started;
      double sp=startPoint;
      uint8_t i=idx;
      for(idx=0;idx<nSeg;idx++) {
        FlowSegment *seg=segments[idx];
        unsigned long l=seg->segTime(sp);
        if (t<=l) {
          point=sp+seg->getPoint(t);
#ifdef DEBUG
            if(idx!=i) report();
#endif
          return true;
        } else {
          sp=seg->target;
          t-=l;
        }
      }
      return false;
    }
    //TODO: this must be absolute, random acces by time to all segments
    //allowing track of the full ideal idealProfile instead of isolated segments
    //this is the classic segmentation, checking for target temperature
    bool _process(double temp,unsigned long t) {
      // Serial<<"process "<<temp<<"ºC "<<t<<"ms #:"<<idx<<" "<<segments[idx]->name<<endl;
      if (segmentProcess(temp,t-segmentStarted)) {
        idx++;
        if(idx<nSeg) {
          segmentStarted=t;
          onTarget=false;
          //chain segments
          //entry point of next is the target of previous
          entryPoint=segments[idx-1]->target;
#ifdef DEBUG
            report();
#endif
          return true;
        } else idx--;
#ifdef DEBUG
          Serial<<endl;
#endif
        return false;
      }
      return  true;
    }
    bool segmentProcess(double temp,unsigned long t) {
      if (!onTarget) {
        if (segments[idx]->reached(temp)) {
#ifdef DEBUG
            Serial<<" reached in "<<t/1000<<"s"<<endl;
#endif
          onTarget=true;
          segmentReached=t;
        } else point=entryPoint+segments[idx]->getPoint(t+updateTime);
      }
      return onTarget&&segments[idx]->rested(t);
    }
    inline double getPoint() const {return point;}
    double startPoint;
    double point;
    double entryPoint;
  protected:
    unsigned long started;
    unsigned long updateTime;
    uint8_t idx=0;
    bool onTarget=false;//segment on target? if so applky segment duration
    unsigned long segmentStarted;//when was this segment started (ms)
    unsigned long segmentReached;//when segment target was reached
#ifdef DEBUG
      void report() {
          Serial<<"#"<<idx<<" "
          <<segments[idx]->name
          <<" slope:"<<segments[idx]->slope*1000.0
          <<"ºC/s target:"<<segments[idx]->target;
          if (driveMode==TimeDrived) Serial<<endl;
      }
#endif
  };
#endif
