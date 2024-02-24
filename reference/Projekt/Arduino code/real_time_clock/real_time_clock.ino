
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <RTCZero.h>

char ssid[] = "Boldi fanatisztikás hotpot-tya";  //  WiFi credentials and setup
char password[] = "32031365";
const int GMT = 2; // Time zone constant - change as required for your location

RTCZero rtc_wifi;
int status = WL_IDLE_STATUS;
byte set_year = 0; // can only be 2-digit year
byte set_month = 0;
byte set_date = 0;
byte set_hour = 0;
byte set_minute = 0;
byte set_second = 0;

void initRealTime() {
  set_year = rtc_wifi.getYear(); // can only be 2-digit year
  set_month = rtc_wifi.getMonth();
  set_date = rtc_wifi.getDay();
  set_hour = rtc_wifi.getHours() + GMT;
  set_minute = rtc_wifi.getMinutes();
  set_second = rtc_wifi.getSeconds();
}


void setup() {
  //Serial.begin(115200);
  //Serial.println("Serial begins!");

  // Establish a WiFi connection
  while ( status != WL_CONNECTED) {
 
    //Serial.print("Attempting to connect to SSID: ");
    //Serial.println(ssid);
    status = WiFi.begin(ssid, password);
 
    // Wait 1 seconds for connection:
    delay(1000);
  }
 
  // Print connection status
  //  printWiFiStatus();
  
  // Start Real Time Clock
  rtc_wifi.begin();
  
  // Variable to represent epoch
  unsigned long epoch;
 
  // Variable for number of tries to NTP service
  int numberOfTries = 0, maxTries = 6;
  
  // Get epoch
  do {
    epoch = WiFi.getTime();
    numberOfTries++;
  }
 
  while ((epoch == 0) && (numberOfTries < maxTries));
 
    if (numberOfTries == maxTries) {
    //Serial.print("NTP unreachable!!");
    while (1);
    }
 
    else {
    //Serial.print("Epoch received: ");
    //Serial.println(epoch);
    rtc_wifi.setEpoch(epoch);
    //Serial.println();
    }

  initRealTime();
  //Serial.println("Initializing real time done.");
  //Serial.println("Online date & time: " + String(set_year) + "-" + String(set_month) + "-" + String(set_date) + " " + String(set_hour) + ":" + String(set_minute) + ":" + String(set_second));

}

void loop(){}