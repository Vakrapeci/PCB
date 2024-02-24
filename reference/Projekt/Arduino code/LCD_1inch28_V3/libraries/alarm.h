// functions for the alarm sequence

#ifndef _ALARM_H_
#define _ALARM_H_

  #define NumberOfBeeps 4     // how many times goes the "beep" sound before the longer silence
  #define LengthOfBeep 75     // ms
  #define LengthOfSilence LengthOfBeep*NumberOfBeeps*2

  void ALARM_LOUD();
  void ALARM_SILENT();


#endif