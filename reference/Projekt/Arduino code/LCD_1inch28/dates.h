// variable structures used by the main program


#ifndef _DATES_H_
#define _DATES_H_

#include <stdint.h>
#include <stdio.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include <TimeLib.h>
#include <RTCZero.h>

struct date {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int weekday;
  unsigned long TIME1;
  unsigned long TIME2;
};

struct alarm {
  String name;  // slot name
  date time;    // time of alarm
  int style;    // 0-OFF  1-vibrate 2-vibrate and sound
  int repeat;   // 0-once 1-weekly  2-monthly
};

void makeTIME(date input);
//void setCurrentTime(date input);
//void setCurrentTime(alarm input);
void setRTCTime(date TO, RTCZero FROM);

void AlarmInit(alarm input[20]);


#endif