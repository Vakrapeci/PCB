#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

#include <TimeLib.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
//#include <BluetoothSerial.h>

#include "alarm.h"
#include "dates.h"
#include "pin_config.h"
#include "print_time.h"
#include "wifi_rtc.h"
//#include "timer_interrupt.h"



// initializing global variables
bool UP;
bool DOWN;
bool LEFT;
bool RIGHT;

int UP_old, DOWN_old, LEFT_old, RIGHT_old;

int mode;
int menu;
int submenu;

jmp_buf env;

int h, m, s;
int s_old;
int inactiveness;
RTCZero rtc_main;

int YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, repeat, style;
int setvalue, which_value, which_slot;
int timer, which_timer;
int THOUR, TMINUTE, TSECOND;

alarm ALARM[20];
date NOW;
String defname = "Slot";
String no;

int active_alarm;

void refreshMenu();

//void Paint_DrawNum2Digit(UWORD Xpoint, UWORD Ypoint, int32_t Nummber, sFONT* Font, UWORD Color_Background, UWORD Color_Foreground );




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
  mode = 0;
  menu = 0;
  submenu = 0;
  UP = DOWN = LEFT = RIGHT = 0;
  UP_old = DOWN_old = LEFT_old = RIGHT_old = 0;
  timer = 300;

  Paint_DrawString_EN(20, 80, "Config done", &Font24, BLACK, RED);
  delay(500);
  Paint_Clear(BLACK);
  LCD_SetBacklight(0);
}




void loop() {
  //setjmp(env);
  ////////////////////////////////////////////////////////////////////
  // things to do in every loop
  ////////////////////////////////////////////////////////////////////

  // set loop variables to current time
  // NOW.hour = rtc_main.getHours();
  // NOW.minute = rtc_main.getMinutes();
  NOW.second = rtc_main.getSeconds();
  // setRTCTime(NOW, rtc_main);

  s = NOW.second;

  if (s != s_old) {
    NOW.year = rtc_main.getYear();
    NOW.month = rtc_main.getMonth();
    NOW.day = rtc_main.getDay();
    NOW.hour = rtc_main.getHours();
    NOW.minute = rtc_main.getMinutes();
    NOW.second = rtc_main.getSeconds();
    NOW.TIME = NOW.hour * 10000 + NOW.minute * 100 + NOW.second;
    NOW.DATE = NOW.year * 10000 + NOW.month * 100 + NOW.day;
  }

  h = NOW.hour;
  m = NOW.minute;

  // // debug
  // Paint_DrawNum(100, 200, mode, &Font24, BLACK, RED);
  // Paint_DrawNum(120, 200, menu, &Font24, BLACK, RED);
  // Paint_DrawNum(140, 200, submenu, &Font24, BLACK, RED);
  // Paint_DrawNum(120, 20, UP, &Font24, BLACK, YELLOW);
  // Paint_DrawNum(200, 120, RIGHT, &Font24, BLACK, YELLOW);
  // Paint_DrawNum(120, 170, DOWN, &Font24, BLACK, YELLOW);
  // Paint_DrawNum(40, 80, LEFT, &Font24, BLACK, YELLOW);

  // Paint_DrawNum(70, 50, ALARM[0].time.TIME, &Font12, BLACK, RED);
  // Paint_DrawNum(70, 60, ALARM[0].time.DATE, &Font12, BLACK, RED);
  // Paint_DrawNum(70, 70, NOW.TIME, &Font12, BLACK, RED);
  // Paint_DrawNum(70, 80, NOW.DATE, &Font12, BLACK, RED);




  // checking if any alarm is due at this second
  for (int i = 0; i < 20; i++) {
    if (NOW.TIME == ALARM[i].time.TIME && NOW.DATE == ALARM[i].time.DATE && ALARM[i].style != 0) {
      mode = 2;
      active_alarm = i;
    }
  }


  // changing mode, menu and submenu according to the pushed button
  if (UP || DOWN || LEFT || RIGHT) {
    refreshMenu();
  }






  ////////////////////////////////////////////////////////////////////
  // sleep mode
  ////////////////////////////////////////////////////////////////////
  if (mode == 0) {
  }


  ////////////////////////////////////////////////////////////////////
  // active mode
  ////////////////////////////////////////////////////////////////////

  // time menu
  if (mode == 1) {

    // time menu
    if (menu == 0) {
      // quick watch submenu
      if (s_old != s && submenu == 0) {
        printDigitalTime(h, m, s);  // display the time on the screen
        inactiveness++;
        if (inactiveness == 10) {  // if the device is inactive for 5s
          mode = 0;                // put it in sleep mode
          menu = 0;                // reset menu and
          submenu = 0;             // submanu
          inactiveness = 0;        // reset timer variable
          LCD_SetBacklight(0);     // turn off LCD
          Paint_Clear(BLACK);      // clear LCD
        }
      }
      // permanent submenu
      if (s_old != s && submenu == 1) {
        printDigitalTime(h, m, s);  // display the time on the screen
      }
    }

    // set alarm menu
    if (menu == 1) {
      // home submenu
      if (submenu == 0) {
        Paint_DrawString_EN(40, 50, "Set alarm", &Font24, BLACK, WHITE);
      }
      // select slot submenu
      if (submenu == 1) {
        switch (which_slot) {
          case 0:
            Paint_DrawString_EN(40, 50, "Slot 1", &Font24, BLACK, WHITE);
            break;
          case 1:
            Paint_DrawString_EN(40, 50, "Slot 2", &Font24, BLACK, WHITE);
            break;
          case 2:
            Paint_DrawString_EN(40, 50, "Slot 3", &Font24, BLACK, WHITE);
            break;
          case 3:
            Paint_DrawString_EN(40, 50, "Slot 4", &Font24, BLACK, WHITE);
            break;
          case 4:
            Paint_DrawString_EN(40, 50, "Slot 5", &Font24, BLACK, WHITE);
            break;
        }
      }
      // set time submenu
      if (submenu == 2) {
        Paint_DrawNum(70, 50, YEAR, &Font24, BLACK, WHITE);
        if (MONTH < 10) {
          Paint_DrawNum(110, 50, 0, &Font24, BLACK, WHITE);
          Paint_DrawNum(127, 50, MONTH, &Font24, BLACK, WHITE);
        } else {
          Paint_DrawNum(110, 50, MONTH, &Font24, BLACK, WHITE);
        }
        if (DAY < 10) {
          Paint_DrawNum(150, 50, 0, &Font24, BLACK, WHITE);
          Paint_DrawNum(167, 50, DAY, &Font24, BLACK, WHITE);
        } else {
          Paint_DrawNum(150, 50, DAY, &Font24, BLACK, WHITE);
        }


        if (HOUR < 10) {
          Paint_DrawNum(70, 108, 0, &Font24, BLACK, WHITE);
          Paint_DrawNum(67, 108, HOUR, &Font24, BLACK, WHITE);
        } else {
          Paint_DrawNum(70, 108, HOUR, &Font24, BLACK, WHITE);
        }
        if (MINUTE < 10) {
          Paint_DrawNum(110, 108, 0, &Font24, BLACK, WHITE);
          Paint_DrawNum(127, 108, MINUTE, &Font24, BLACK, WHITE);
        } else {
          Paint_DrawNum(110, 108, MINUTE, &Font24, BLACK, WHITE);
        }
        if (SECOND < 10) {
          Paint_DrawNum(150, 108, 0, &Font24, BLACK, WHITE);
          Paint_DrawNum(167, 108, SECOND, &Font24, BLACK, WHITE);
        } else {
          Paint_DrawNum(150, 108, SECOND, &Font24, BLACK, WHITE);
        }

        Paint_DrawNum(70, 135, repeat, &Font24, BLACK, WHITE);
        Paint_DrawNum(150, 135, style, &Font24, BLACK, WHITE);

        switch (which_value) {
          case 0:  // year
            Paint_ClearWindows(70, 50, 100, 75, BLACK);
            break;
          case 1:  // month
            Paint_ClearWindows(110, 50, 140, 75, BLACK);
            break;
          case 2:  // day
            Paint_ClearWindows(150, 50, 180, 75, BLACK);
            break;
          case 3:  // hour
            Paint_ClearWindows(70, 108, 100, 132, BLACK);
            break;
          case 4:  // minute
            Paint_ClearWindows(110, 108, 140, 132, BLACK);
            break;
          case 5:  // second
            Paint_ClearWindows(150, 108, 180, 132, BLACK);
            break;
          case 6:  // repeat
            Paint_ClearWindows(70, 135, 100, 160, BLACK);
            break;
          case 7:  // style
            Paint_ClearWindows(150, 135, 180, 160, BLACK);
            break;
        }
      }
    }

    // manage alarms menu
    if (menu == 2) {
      LCD_SetBacklight(255);
      printDigitalTime(h, m, s);
    }

    // settings menu
    if (menu == 3) {
      LCD_SetBacklight(255);
      if (submenu == 0) {
        Paint_DrawString_EN(60, 50, "Settings", &Font24, BLACK, WHITE);
      }
      if (submenu == 1) {
        Paint_DrawString_EN(60, 50, "Clock type", &Font24, BLACK, WHITE);
      }
      if (submenu == 2) {
        Paint_DrawString_EN(60, 50, "Bluetooth", &Font24, BLACK, WHITE);
      }
    }

    // timer menu
    if (menu == 4) {
      LCD_SetBacklight(255);
      Paint_DrawString_EN(40, 50, "Timer", &Font24, BLACK, WHITE);
      // home submenu
      if (submenu == 0) {
      }
      // set timer submenu
      if (submenu == 1) {

        if (HOUR < 10) {
          Paint_DrawNum(70, 108, 0, &Font24, BLACK, WHITE);
          Paint_DrawNum(67, 108, THOUR, &Font24, BLACK, WHITE);
        } else {
          Paint_DrawNum(70, 108, THOUR, &Font24, BLACK, WHITE);
        }
        if (MINUTE < 10) {
          Paint_DrawNum(110, 108, 0, &Font24, BLACK, WHITE);
          Paint_DrawNum(127, 108, TMINUTE, &Font24, BLACK, WHITE);
        } else {
          Paint_DrawNum(110, 108, TMINUTE, &Font24, BLACK, WHITE);
        }
        if (SECOND < 10) {
          Paint_DrawNum(150, 108, 0, &Font24, BLACK, WHITE);
          Paint_DrawNum(167, 108, TSECOND, &Font24, BLACK, WHITE);
        } else {
          Paint_DrawNum(150, 108, TSECOND, &Font24, BLACK, WHITE);
        }

        switch (which_timer) {
          case 0:  // hour
            Paint_ClearWindows(70, 108, 100, 132, BLACK);
            break;
          case 1:  // minute
            Paint_ClearWindows(110, 108, 140, 132, BLACK);
            break;
          case 2:  // second
            Paint_ClearWindows(150, 108, 180, 132, BLACK);
            break;
        }
      }
      // timer countback submenu
      if (submenu == 2) {
        if (s != s_old) {
          Paint_DrawString_EN(50, 50, "Time left", &Font24, BLACK, WHITE);
          //printDigitalTime(THOUR, TMINUTE, TSECOND);
          Paint_ClearWindows(100, 100, 200, 124, BLACK);
          Paint_DrawNum(100, 100, timer, &Font24, BLACK, WHITE);
          timer--;
          if (timer == 0) {
            while (!digitalRead(OFFBTN)) {
              ALARM_LOUD();
              Paint_DrawString_EN(50, 110, "Timer", &Font24, RED, BLACK);
            }
            menu = 0;
            submenu = 0;
          }
        }
      }
    }
  }

  ////////////////////////////////////////////////////////////////////
  // alarm mode
  ////////////////////////////////////////////////////////////////////
  if (mode == 2) {
    LCD_SetBacklight(255);
    if (ALARM[active_alarm].style == 1) {
      while (!digitalRead(OFFBTN)) {
        ALARM_SILENT();
        printDigitalTime(h, m, s, RED, BLACK);
      }
      if (ALARM[active_alarm].repeat == 1) {
        ALARM[active_alarm].time.day += 1;
        ALARM[active_alarm].time.DATE += 1;
      }
      if (ALARM[active_alarm].repeat == 2) {
        ALARM[active_alarm].time.day += 7;
        ALARM[active_alarm].time.DATE += 7;
      }
      if (ALARM[active_alarm].repeat == 3) {
        ALARM[active_alarm].time.month += 1;
        ALARM[active_alarm].time.DATE += 100;
      }
      mode = 1;
      menu = 0;
      submenu = 0;
      Paint_Clear(BLACK);
      Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    }
    if (ALARM[active_alarm].style == 2) {
      while (!digitalRead(OFFBTN)) {
        ALARM_LOUD();
        printDigitalTime(h, m, s, RED, BLACK);
      }
      mode = 1;
      menu = 0;
      submenu = 0;
      Paint_Clear(BLACK);
      Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    }
  }

  //delay(10);

  s_old = s;
}


//////////////////////////////////
// function for refreshing
// the mode, menu and submenu
/////////////////////////////////

void refreshMenu() {
  // if the UBTN was pushed
  if (UP) {
    switch (mode) {
      case 0:      // if the device is in sleep mode
        mode = 1;  // put it in active mode
        LCD_SetBacklight(255);
        Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
        break;
      case 1:  // if the device is in active mode
        switch (menu) {
          case 0:  // time menu
            switch (submenu) {
              case 0:         // quick watch submenu
                submenu = 1;  // put it in permanent submenu
                break;
              case 1:         // permanent submenu
                mode = 0;     // put it in sleep mode
                menu = 0;     // reset menu and
                submenu = 0;  // submenu
                LCD_SetBacklight(0);
                Paint_Clear(BLACK);
                break;
            }
            break;
          case 1:  // set alarm menu
            switch (submenu) {
              // home submenu
              case 0:  // sleep mode
                mode = 0;
                menu = 0;
                submenu = 0;
                LCD_SetBacklight(0);
                Paint_Clear(BLACK);
                break;
              // select slot submenu
              case 1:  // home submenu
                submenu = 0;
                Paint_ClearWindows(20, 100, 220, 140, BLACK);
                break;
              // set time submenu
              case 2:  // add 1 to current value
                //setvalue++;
                switch (which_value) {
                  case 0:  // year
                    YEAR++;
                    break;
                  case 1:  // month
                    MONTH %= 12;
                    MONTH++;
                    break;
                  case 2:  // day
                    DAY %= 31;
                    DAY++;
                    break;
                  case 3:  // hour
                    HOUR++;
                    HOUR %= 24;
                    break;
                  case 4:  // minute
                    MINUTE++;
                    MINUTE %= 60;
                    break;
                  case 5:  // second
                    SECOND++;
                    SECOND %= 60;
                    break;
                  case 6:  // repeat
                    repeat++;
                    repeat %= 4;
                    break;
                  case 7:  // style
                    style++;
                    style %= 3;
                    break;
                }
                break;
            }
            break;
          case 2:  // manage alarms menu
            break;
          case 3:  // settings menu
            break;
          case 4:  // timer menu
            switch (submenu) {
              case 0:
                mode = 0;
                menu = 0;
                submenu = 0;
                LCD_SetBacklight(0);
                Paint_Clear(BLACK);
                break;
              // set timer submenu
              case 1:
                switch (which_timer) {
                  case 0:  // hour
                    THOUR++;
                    THOUR %= 100;
                    break;
                  case 1:  // minute
                    TMINUTE++;
                    TMINUTE %= 60;
                    break;
                  case 2:  // second
                    TSECOND++;
                    TSECOND %= 60;
                    break;
                }
            }
            break;
        }
        // case 2:   // if the device is in alarm mode
        //   mode = 1;   // put it back to active mode
        //   menu = 0;   // time menu
        //   submenu = 0;// quick watch submenu
        //   break;
    }
  }
  // if DBTN was pushed
  if (DOWN) {
    switch (mode) {
      //if the device is in sleep mode
      case 0:
        mode = 1;  // put it in active mode
        LCD_SetBacklight(255);
        Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
        break;

      //if the device is in active mode
      case 1:
        switch (menu) {
          //time menu
          case 0:
            switch (submenu) {
              // quick watch submenu
              case 0:  // put it in permanent submenu
                submenu = 1;
                break;
              // permanent submenu
              case 1:  // put it in sleep mode
                mode = 0;
                menu = 0;     // reset menu and
                submenu = 0;  // submenu
                LCD_SetBacklight(0);
                Paint_Clear(BLACK);
                break;
            }
            break;

          // set alarm menu
          case 1:
            switch (submenu) {
                // home submenu
              case 0:  // select slot submenu
                submenu = 1;
                which_slot = 0;
                Paint_ClearWindows(27, 50, 215, 74, BLACK);
                Paint_ClearWindows(16, 111, 215, 130, BLACK);
                break;
              // select slot submenu
              case 1:  // set time submenu
                submenu = 2;
                which_value = 3;
                YEAR = NOW.year;
                MONTH = NOW.month;
                DAY = NOW.day;
                HOUR = NOW.hour;
                MINUTE = NOW.minute;
                SECOND = NOW.second;
                repeat = 0;  // default: once
                style = 2;   // default: loud alarm
                Paint_ClearWindows(27, 50, 215, 74, BLACK);
                Paint_ClearWindows(16, 111, 215, 130, BLACK);
                break;
              // set time submenu
              case 2:  // decrease current value by 1
                //setvalue--;
                switch (which_value) {
                  case 0:  // year
                    YEAR--;
                    break;
                  case 1:  // month
                    MONTH--;
                    if (MONTH == 0) { MONTH = 12; }
                    break;
                  case 2:  // day
                    DAY--;
                    if (DAY == 0) { DAY = 31; }
                    break;
                  case 3:  // hour
                    HOUR--;
                    if (HOUR == -1) { HOUR = 23; }
                    break;
                  case 4:  // minute
                    MINUTE--;
                    if (MINUTE == -1) { MINUTE = 59; }
                    break;
                  case 5:  // second
                    SECOND--;
                    if (SECOND == -1) { SECOND = 59; }
                    break;
                  case 6:  // repeat
                    repeat--;
                    if (repeat == -1) { repeat = 3; }
                    break;
                  case 7:  // style
                    style--;
                    if (style == -1) { style = 2; }
                    break;
                }
                break;
            }
            break;
          //manage alarms menu
          case 2:
            switch (submenu) {
              case 0:
                submenu = 1;
                break;
              case 1:
                break;
            }
            break;
          //settings menu*/ 
          case 3:
            switch (submenu) {
              case 0:
                submenu = 1;
                break;
              case 1:
                break;
            }
            break;
          // timer menu
          case 4:
            switch (submenu) {
              // home submenu
              case 0:
                submenu = 1;
                THOUR = TMINUTE = TSECOND = 0;
                which_timer = 1;
                break;
              // set timer submenu
              case 1:
                switch (which_timer) {
                  case 0:  // hour
                    THOUR--;
                    if (THOUR == -1) { THOUR = 99; }
                    break;
                  case 1:  // minute
                    TMINUTE--;
                    if (TMINUTE == -1) { TMINUTE = 59; }
                    break;
                  case 2:  // second
                    TSECOND--;
                    if (SECOND == -1) { TSECOND = 59; }
                    break;
                }
                break;
            }
        }

      //if the device is in alarm mode
      case 2:
        break;
    }
  }
  // if LBTN was pushed
  if (LEFT) {
      switch (mode) {
        case 0:      // if the device is in sleep mode
          mode = 1;  // put it in active mode
          LCD_SetBacklight(255);
          Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
          break;
        // if the device is in active mode
        case 1:
          switch (menu) {
            // time menu
            case 0:
              menu--;  // change menu to the left
              if (menu == -1) { menu += 5; }
              Paint_ClearWindows(27, 50, 215, 74, BLACK);
              Paint_ClearWindows(16, 111, 215, 130, BLACK);
              //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
              //menu %= 5;          // put the menu value back to {0,1,2,3,4,5}, if it becomes lower than 0
              break;
            // set alarm menu
            case 1:
              switch (submenu) {
                // home submenu
                case 0:
                  menu--;
                  if (menu == -1) { menu += 5; }
                  Paint_ClearWindows(27, 50, 215, 74, BLACK);
                  Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
                  //menu %= 5;
                  break;
                // select slot submenu
                case 1:
                  which_slot--;
                  if (which_slot == -1) { which_slot = 4; }
                  break;
                // set time submenu
                case 2:
                  which_value--;
                  if (which_value == -1) {
                    submenu = 1;
                    which_value = 3;  // default which_value is the hour
                    //Paint_ClearWindows(10,10,20,20,BLACK);
                  }
                  break;
              }
              break;
            // manage alarms menu
            case 2:
              switch (submenu) {
                case 0:  // step submenu
                  menu--;
                  if (menu == -1) { menu += 5; }
                  Paint_ClearWindows(27, 50, 215, 74, BLACK);
                  Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
                  //menu %= 5;
                  break;
                case 1:
                  break;
              }
              break;
            // settings menu
            case 3:
              switch (submenu) {
                case 0:  // step submenu
                  menu--;
                  if (menu == -1) { menu += 5; }
                  Paint_ClearWindows(27, 50, 215, 74, BLACK);
                  Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
                  //menu %= 5;
                  break;
                case 1:
                  break;
              }
              break;
            // timer menu
            case 4:
              switch (submenu) {
                case 0:   // home submenu
                  menu--;
                  if(menu == -1){menu = 4;}
                  Paint_ClearWindows(27, 50, 215, 74, BLACK);
                  Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  break;
                case 1:   // set timer submenu
                  which_timer--;
                  if (which_timer == -1) { 
                    submenu = 0;
                  }
                  Paint_ClearWindows(27, 50, 215, 74, BLACK);
                  Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
                  //menu %= 5;
                  break;
                case 2:
                  break;
              }
              break;
          }
        case 2:  // if the device is in alarm mode
          break;
      }
    }
    // if RBTN was pushed
    if (RIGHT) {
      switch (mode) {
        case 0:      // if the device is in sleep mode
          mode = 1;  // put it in active mode
          LCD_SetBacklight(255);
          Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
          break;
        case 1:  // if the device is in active mode
          switch (menu) {
            case 0:       // time menu
              menu++;     // change menu to the left
              menu %= 5;  // put the menu value back to {0,1,2,3,4,5}, if it becomes lower than 0
              Paint_ClearWindows(27, 50, 215, 74, BLACK);
              Paint_ClearWindows(16, 111, 215, 130, BLACK);
              break;
            case 1:  // set alarm menu
              switch (submenu) {
                // chose alarm submenu
                case 0:
                  menu++;  // next menu
                  menu %= 5;
                  Paint_ClearWindows(27, 50, 215, 74, BLACK);
                  break;
                // select slot submenu
                case 1:  // next slot
                  which_slot++;
                  which_slot %= 5;
                  Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  break;
                // set time submenu
                case 2:  // nex value to set
                  which_value++;
                  if (which_value == 8) {  // save
                    ALARM[which_slot].time.year = YEAR;
                    ALARM[which_slot].time.month = MONTH;
                    ALARM[which_slot].time.day = DAY;
                    ALARM[which_slot].time.hour = HOUR;
                    ALARM[which_slot].time.minute = MINUTE;
                    ALARM[which_slot].time.second = SECOND;
                    ALARM[which_slot].time.TIME = 10000 * HOUR + 100 * MINUTE + SECOND;
                    ALARM[which_slot].time.DATE = 10000 * YEAR + 100 * MONTH + DAY;
                    ALARM[which_slot].style = style;
                    ALARM[which_slot].repeat = repeat;

                    mode = 0;
                    menu = 0;
                    submenu = 0;
                    Paint_Clear(BLACK);
                    Paint_DrawString_EN(50, 50, "Alarm saved", &Font20, BLACK, WHITE);
                    delay(500);
                    LCD_SetBacklight(0);
                    Paint_Clear(BLACK);
                  }
                  break;
              }
              break;
            case 2:  // manage alarms menu
              switch (submenu) {
                case 0:  // step submenu
                  menu++;
                  menu %= 5;
                  Paint_ClearWindows(27, 50, 215, 74, BLACK);
                  Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  break;
                case 1:
                  break;
              }
              break;
            case 3:  // settings menu
              switch (submenu) {
                case 0:  // step submenu
                  menu++;
                  menu %= 5;
                  Paint_ClearWindows(27, 50, 215, 74, BLACK);
                  Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  break;
                case 1:
                  break;
              }
              break;
            case 4:  // timer menu
              switch (submenu) {
                // home submenu
                case 0:
                  menu++;
                  menu %= 5;
                  Paint_ClearWindows(27, 50, 215, 74, BLACK);
                  Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  break;
                // set timer submenu
                case 1:
                  which_timer++;
                  if(which_timer == 3){
                    timer = THOUR * 60 * 60 + TMINUTE * 60 + TSECOND;
                    submenu = 2;
                    Paint_ClearWindows(27, 50, 215, 74, BLACK);
                    Paint_ClearWindows(16, 111, 215, 130, BLACK);
                  }
                  break;
                case 2:
                  break;
              }
              break;
          }
        case 2:  // if the device is in alarm mode
          break;
      }
    }

    // reset button variables
    UP = LOW;
    DOWN = LOW;
    RIGHT = LOW;
    LEFT = LOW;
  }



  //////////////////////////////////
  // push button interrupt functions
  //////////////////////////////////

  void ISR_UP() {
    UP = HIGH;
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
    DOWN = HIGH;
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
    LEFT = HIGH;
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
    RIGHT = HIGH;
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

  // void Paint_DrawNum2Digit(UWORD Xpoint, UWORD Ypoint, int32_t Nummber,
  //                    sFONT* Font, UWORD Color_Background, UWORD Color_Foreground )
  // {
  //   if(Nummber < 10){
  //     switch(Font){
  //       case &Font8:
  //         int x = 5;
  //         break;
  //       case &Font12:
  //         int x = 7;
  //         break;
  //       case &Font16:
  //         int x = 11;
  //         break;
  //       case &Font20:
  //         int x = 14;
  //         break;
  //       case &Font24:
  //         int x = 17;
  //         break;
  //     }
  //     Paint_DrawNum(Xpoint, Ypoint, 0, Font, Color_Background, Color_Foreground);
  //     Paint_DrawNum(Xpoint + x, Ypoint, Number, Font, Color_Background, Color_Foreground)
  //   }else{
  //     Paint_DrawNum(Xpoint, Ypoint, 0, Font, Color_Background, Color_Foreground);
  //   }
  // }



  /*********************************************************************************************************
  END FILE
*********************************************************************************************************/
