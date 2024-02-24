#include "wifi_rtc.h"


char ssid[] = "Projekt";  //  WiFi credentials and setup
char password[] = "32031365";
const int GMT = 2; // Time zone constant - change as required for your location
unsigned long epoch;
int status = WL_IDLE_STATUS;

RTCZero rtc;


void wifi_connect(){
  // Establish a WiFi connection
  while ( status != WL_CONNECTED) {    
    status = WiFi.begin(ssid, password);
    delay(1000);
  }
  Paint_DrawString_EN(20, 50, "WiFi connection", &Font16, BLACK, RED);
  Paint_DrawString_EN(20, 70, "established", &Font16, BLACK, RED);
  delay(500);
  Paint_Clear(BLACK);
}

void getRTCto(RTCZero rtc){
  
  // Variable for number of tries to NTP service
  int numberOfTries = 0, maxTries = 6;  

  // Get epoch
  do {
    epoch = WiFi.getTime();
    numberOfTries++;
    delay(500);
  } 
  while ((epoch == 0) && (numberOfTries < maxTries));
 
  if (numberOfTries == maxTries) {
    Paint_DrawString_EN(20, 50, "NTP unreachable", &Font16, BLACK, RED);
    delay(500);
    Paint_Clear(BLACK);
  }else {
    Paint_DrawString_EN(20, 50, "Epoch recieved:", &Font16, BLACK, RED);
    Paint_DrawNum(20, 70, epoch, &Font16, BLACK, RED);
    
    rtc.setEpoch(epoch);
    rtc.setHours(rtc.getHours() + GMT);
    delay(500);
    Paint_Clear(BLACK);
  }
}