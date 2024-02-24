// variable structures used by main


#include "dates.h"



//RTCZero rtc;

void makeTIME(date input){
  input.TIME = input.hour * 10000 +
                input.minute * 100 +
                input.second;
  input.DATE = input.year * 10000 +
                input.month * 100 +
                input.day;
}

/*
void setCurrentTime(date input){
  input.year = rtc.getYear();
  input.month = rtc.getMonth();
  input.day = rtc.getDay();
  input.hour = rtc.getHours();
  input.minute = rtc.getMinutes();
  input.second = rtc.getSeconds();
  //input.weekday = weekday();
  makeTIME(input);
}

void setCurrentTime(alarm input){
  input.time.year     = year();
  input.time.month    = month();
  input.time.day      = day();
  input.time.hour     = hour();
  input.time.minute   = minute();
  input.time.second   = second();
  input.time.weekday  = weekday();
  makeTIME(input.time);
}*/

void setRTCTime(date TO, RTCZero FROM){
  TO.year   = FROM.getYear();
  TO.month  = FROM.getMonth();
  TO.day    = FROM.getDay();
  TO.hour   = FROM.getHours();
  TO.minute = FROM.getMinutes();
  TO.second = FROM.getSeconds();
  TO.TIME =  TO.hour * 10000 +
              TO.minute * 100 +
              TO.second;
  TO.DATE =  TO.year * 10000 +
              TO.month * 100 +
              TO.day;
}

