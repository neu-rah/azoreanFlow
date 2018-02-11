  #include "profiles.h"

// using namespace Sn965_Ag30_Cu05;
FlowSegment* Sn965_Ag30_Cu05::segments[]={
  new FlowSegment("SOAK ramp",TEMPERATURE_ROOM,150,90),
  new FlowSegment("PreHeat-SOAK",150,175,90),
  new FlowSegment("PEAK ramp",  175,217,30),
  new FlowSegment("REFLOW in",  217,249,30),
  new FlowSegment("REFLOW out",249,217,30)
};

AzoreanFlow<5> Sn965_Ag30_Cu05::flow(TEMPERATURE_ROOM,segments,PID_SAMPLE_TIME);

FlowSegment* SnDEBUG::segments[]={
  new FlowSegment("SOAK Ramp",TEMPERATURE_ROOM,50,10),
  new FlowSegment("PreHeat-SOAK",50,75,30),
  new FlowSegment("PEAK Ramp",  75,90,15),
  new FlowSegment("REFLOW in",  90,119,15),
  new FlowSegment("REFLOW out",119,90,15)
};

AzoreanFlow<5> SnDEBUG::flow(TEMPERATURE_ROOM,segments,PID_SAMPLE_TIME);

FlowSegment* Sn965_Ag30_Cu05_smooth::segments[]={
  new FlowSegment("SOAK ramp",TEMPERATURE_ROOM,150,130),
  new FlowSegment("PreHeat-SOAK",150,175,90),
  new FlowSegment("PEAK ramp",  175,217,60),
  new FlowSegment("REFLOW in",  217,249,35),
  new FlowSegment("REFLOW out",249,217,80)
};

AzoreanFlow<5> Sn965_Ag30_Cu05_smooth::flow(TEMPERATURE_ROOM,segments,PID_SAMPLE_TIME);
