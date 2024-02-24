#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

#include <TimeLib.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>

#include "alarm.h"
#include "dates.h"
#include "pin_config.h"
#include "print_time.h"
#include "wifi_rtc.h"
//#include "timer_interrupt.h"



// initializing global variables
volatile bool UP;
volatile bool DOWN;
volatile bool LEFT;
volatile bool RIGHT;

int UP_old, DOWN_old, LEFT_old, RIGHT_old;

int mode;
int menu;
int submenu;
int mode_old;

jmp_buf env;

int h, m, s;
int s_old;
int inactiveness;
RTCZero rtc_main;

alarm ALARM[20];
date NOW;
String defname = "Slot";
String no;

int active_alarm;




void setup() {
  //LDC setup
  Config_Init();
  LCD_Init();
  LCD_SetBacklight(255);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
  Paint_Clear(BLACK);
  //Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  //Paint_DrawLine  (120, 0, 120, 12, GREEN, DOT_PIXEL_4X4, LINE_STYLE_SOLID);
  //Paint_DrawLine  (120, 228, 120, 240, GREEN ,DOT_PIXEL_4X4, LINE_STYLE_SOLID);
  //Paint_DrawLine  (0, 120, 12, 120, GREEN, DOT_PIXEL_4X4, LINE_STYLE_SOLID);
  //Paint_DrawLine  (228, 120, 240, 120, GREEN, DOT_PIXEL_4X4, LINE_STYLE_SOLID);
  //Paint_DrawImage (gImage_70X70, 85, 25, 70, 70);
  //Paint_DrawString_EN(123, 123, "WAVESHARE", &Font16, BLACK, GREEN);
  //Paint_DrawLine  (120, 120, 70, 70, YELLOW, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
  //Paint_DrawLine  (120, 120, 176, 64, BLUE, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
  //Paint_DrawLine  (120, 120, 120, 210, RED, DOT_PIXEL_2X2, LINE_STYLE_SOLID);



  // wifi connection, RTC from NTP server
  wifi_connect();
  rtc_main.begin();
  getRTCto(rtc_main);
  setRTCTime(NOW, rtc_main);
  //makeTime(NOW);

  pinSetup();
  AlarmInit(ALARM);
  
  // interrupt button functions
  attachInterrupt(digitalPinToInterrupt(UBTN), ISR_UP, FALLING);
  attachInterrupt(digitalPinToInterrupt(DBTN), ISR_DOWN, FALLING);
  attachInterrupt(digitalPinToInterrupt(LBTN), ISR_LEFT, FALLING);
  attachInterrupt(digitalPinToInterrupt(RBTN), ISR_RIGHT, FALLING);

  // setting up initial menu (quick watch menu)
  mode = 1;
  menu = 0;
  submenu = 0;
  mode_old = 1;
  UP = DOWN = LEFT = RIGHT = 0;
  UP_old = DOWN_old = LEFT_old = RIGHT_old = 0;

  Paint_DrawString_EN(80, 80, "Config done", &Font24, BLACK, RED);
  delay(1000);
  Paint_Clear(BLACK);
}




void loop() {
  setjmp(env);
  ////////////////////////////////////////////////////////////////////
  // things to do in every loop
  ////////////////////////////////////////////////////////////////////

  // // set loop variables to current time
  // NOW.hour = rtc_main.getHours();
  // NOW.minute = rtc_main.getMinutes();
  // NOW.second = rtc_main.getSeconds();
  // setRTCTime(NOW, rtc_main);
  NOW.year = rtc_main.getYear();
  NOW.month = rtc_main.getMonth();
  NOW.day = rtc_main.getDay();
  NOW.hour = rtc_main.getHours();
  NOW.minute = rtc_main.getMinutes();
  NOW.second = rtc_main.getSeconds();
  NOW.TIME1 = NOW.hour * 10000 + 
              NOW.minute * 100 + 
              NOW.second;
  NOW.TIME2 = NOW.year * 10000 + 
              NOW.month * 100 + 
              NOW.day;

  h = NOW.hour;
  m = NOW.minute;
  s = NOW.second;

  printDigitalTime(h, m, s);
  Paint_DrawNum(40, 160, NOW.TIME1, &Font24, BLACK, RED);
  Paint_DrawNum(120, 200, mode, &Font24, BLACK, RED);

  // if the time has changed, refresh the NOW variable
  if (s != s_old) {
    setRTCTime(NOW, rtc_main);
  }

  // checking if any alarm is due at this second
  for (int i = 0; i < 20; i++) {
    if (NOW.TIME1 == ALARM[i].time.TIME1 || ALARM[i].style != 0) {
      mode = 2;
      active_alarm = i;
    }
  }


  ////////////////////////////////////////////////////////////////////
  // sleep mode
  ////////////////////////////////////////////////////////////////////
  if (mode == 0) {
    //if (mode_old != 0){LCD_SetBacklight(0);}
    if (UP != UP_old || DOWN != DOWN_old || RIGHT != RIGHT_old || LEFT != LEFT_old){
      Paint_Clear(BLACK);
      LCD_SetBacklight(255);
      Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
      mode = 1;
    }
  }


  ////////////////////////////////////////////////////////////////////
  // active mode
  ////////////////////////////////////////////////////////////////////

  // time menu
  if (mode == 1) {
    // checking if left or right button was pushed
    if (RIGHT != RIGHT_old){    // if right button was pushed
      menu++;
      menu %= 5;
      RIGHT_old = RIGHT;
    }
    if (LEFT != LEFT_old){      // if left button was pushed
      menu--;
      menu %= 5;
      LEFT_old = LEFT;
    }

    // time menu
    if (menu == 0) {
      // quick watch submenu
      if (s_old != s && submenu == 0) {
        printDigitalTime(h, m, s);  // display the time on the screen
        inactiveness++;
        if (inactiveness == 10) {   // if the device is inactive for 5s
          mode = 0;                 // put it in sleep mode
          menu = 0;                 // reset menu and
          submenu = 0;              // submanu
          inactiveness = 0;
          LCD_SetBacklight(0);      // turn off LCD
        }        
        if (UP != UP_old){
          submenu = 1;
        }
      }
      //permanent submenu
      if (s_old != s && submenu == 1) {
        printDigitalTime(h, m, s);  // display the time on the screen
      }      
      if (UP != UP_old){
        LCD_SetBacklight(0);
        mode = 0;
      }
    }


    // set alarm menu
    if (menu == 1) {
      LCD_SetBacklight(255);
      Paint_Clear(BLACK);
      printDigitalTime(h, m, s);
      Paint_DrawNum(40, 160, NOW.TIME1, &Font24, BLACK, RED);
      Paint_DrawNum(120, 200, mode, &Font24, BLACK, RED);
    }

    // manage alarms menu
    if (menu == 2) {
      LCD_SetBacklight(255);
      Paint_Clear(BLACK);
      printDigitalTime(h, m, s);
      Paint_DrawNum(40, 160, NOW.TIME1, &Font24, BLACK, RED);
      Paint_DrawNum(120, 200, mode, &Font24, BLACK, RED);
    }

    // settings menu
    if (menu == 3) {
      LCD_SetBacklight(255);
      Paint_Clear(BLACK);
      printDigitalTime(h, m, s);
      Paint_DrawNum(40, 160, NOW.TIME1, &Font24, BLACK, RED);
      Paint_DrawNum(120, 200, mode, &Font24, BLACK, RED);
    }

    // timer menu
    if (menu == 4) {
      LCD_SetBacklight(255);
      Paint_Clear(BLACK);
      printDigitalTime(h, m, s);
      Paint_DrawNum(40, 160, NOW.TIME1, &Font24, BLACK, RED);
      Paint_DrawNum(120, 200, mode, &Font24, BLACK, RED);
    }
  }

  ////////////////////////////////////////////////////////////////////
  // alarm mode
  ////////////////////////////////////////////////////////////////////
  if (mode == 2) {
    if (ALARM[active_alarm].style == 1) {
      while (!digitalRead(OFFBTN)){
        ALARM_SILENT();
        printDigitalTime(h, m, s, RED, BLACK);
        // Paint_DrawNum(40, 160, NOW.TIME1, &Font24, RED, BLACK);
        // Paint_DrawNum(120, 200, mode, &Font24, RED, BLACK);
      }
    }
    if (ALARM[active_alarm].style == 2) {
      while (!digitalRead(OFFBTN)){
        ALARM_LOUD();
        printDigitalTime(h, m, s, RED, BLACK);
        // Paint_DrawNum(40, 160, NOW.TIME1, &Font24, RED, BLACK);
        // Paint_DrawNum(120, 200, mode, &Font24, RED, BLACK);
      }
    }
  }

  delay(10);

  s_old = s;
  // mode_old = mode;
  // UP_old = UP;
  // DOWN_old = DOWN;
  // RIGHT_old = RIGHT;
  // LEFT_old = LEFT;
}


//////////////////////////////////
// push button interrupt functions
//////////////////////////////////

void ISR_UP() { 
  UP = !UP;
  /*
  switch (mode){  
  case 0:   // if the device is in sleep mode                   
    mode == 1;            // put it in active mode
    break;
  case 1:   // if the device is in active mode                 
    switch (menu){    
    case 0:     // time menu                   
      switch (submenu){      
      case 0:   // quick watch submenu   
        submenu = 1;      // put it in permanent submenu
        break;      
      case 1:   // permanent submenu  
        mode = 0;         // put it in sleep mode
        menu = 0;         // reset menu and
        submenu = 0;      // submenu
        break;
      }
      break;
    case 1:   // set alarm menu
      break;
    case 2:   // manage alarms menu
      break;
    case 3:   // settings menu
      break;
    case 4:   // timer menu
      break;
    }
  case 2:   // if the device is in alarm mode
    break;
  }
  longjmp(env, 1);*/
}


void ISR_DOWN() {
  DOWN = !DOWN;
  /*
  switch (mode){  
  case 0:   // if the device is in sleep mode                   
    mode == 1;            // put it in active mode
    break;
  case 1:   // if the device is in active mode                 
    switch (menu){    
    case 0:     // time menu                   
      switch (submenu){      
      case 0:       // quick watch submenu   
        submenu = 1;      // put it in permanent submenu
        break;      
      case 1:       // permanent submenu  
        mode = 0;         // put it in sleep mode
        menu = 0;         // reset menu and
        submenu = 0;      // submenu
        break;
      }
      break;
    case 1:     // set alarm menu
      break;
    case 2:     // manage alarms menu
      break;
    case 3:     // settings menu
      break;
    case 4:     // timer menu
      break;
    }
  case 2:   // if the device is in alarm mode
    break;
  }*/
}


void ISR_LEFT() {  
  LEFT = !LEFT;
  /*
  switch (mode){  
  case 0:   // if the device is in sleep mode                   
    mode == 1;            // put it in active mode
    break;
  case 1:   // if the device is in active mode                 
    switch (menu){    
    case 0:     // time menu
      menu--;             // change menu to the left
      menu %= 5;          // put the menu value back to {0,1,2,3,4,5}, if it becomes lower than 0
      break;
    case 1:     // set alarm menu
      switch (submenu){
      case 0:       // chose alarm submenu
        menu--;
        menu %= 5;
        break;
      case 1:
        break;
      }
      break;
    case 2:     // manage alarms menu
      break;
    case 3:     // settings menu
      break;
    case 4:     // timer menu
      break;
    }
  case 2:   // if the device is in alarm mode
    break;
  }

  // if the device is in active mode ->
  // time menu
  if (menu == 1) {
  }*/
}


void ISR_RIGHT() {
  RIGHT = !RIGHT;
  /*
  // if the device is in sleep mode
  if (mode == 0) {
    mode == 1;              // put it in time display mode
    LCD_SetBacklight(255);  // turn on LCD
    Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  }

  // if the device is in active mode ->
  // time menu
  if (menu == 1) {
    menu++;     // change menu to the right
    menu %= 5;  // put the menu value back to {0,1,2,3,4,5}, if it becomes greater than 5
  }*/
}




void AlarmInit(alarm input[20]) {
  for (int i = 0; i < 20; i++) {
    no = String(i - 1);
    ALARM[i].name = defname + no;
  }
}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
