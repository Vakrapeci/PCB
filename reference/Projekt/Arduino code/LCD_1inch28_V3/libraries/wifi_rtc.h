#ifndef _WIFI_RTC_H_
#define _WIFI_RTC_H_


#include "LCD_Driver.h"
#include "GUI_Paint.h"

#include <RTCZero.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>


void wifi_connect();
void getRTCto(RTCZero rtc);


#endif