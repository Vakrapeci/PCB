#include <SPI.h>
#include "libraries/LCD_Driver.h"
#include "libraries/GUI_Paint.h"
#include "libraries/image.h"

#include <TimeLib.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
//#include <BluetoothSerial.h>

#include "libraries/alarm.h"
#include "libraries/dates.h"
#include "libraries/pin_config.h"
#include "libraries/print_time.h"
#include "libraries/wifi_rtc.h"
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

alarm ALARM[5];
date NOW;
String defname = "Slot";
String no;

int active_alarm;

void refreshMenu();


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

  Paint_DrawString_EN(20, 80, "Config done", &Font24, BLACK, RED);
  delay(500);
  Paint_Clear(BLACK);
  LCD_SetBacklight(0);

  ALARM[0].time.TIME = 35100;
  ALARM[0].time.DATE = 230606;
  ALARM[0].style = 2;
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

  // debug
  Paint_DrawNum(100, 200, mode, &Font24, BLACK, RED);
  Paint_DrawNum(120, 200, menu, &Font24, BLACK, RED);
  Paint_DrawNum(140, 200, submenu, &Font24, BLACK, RED);
  Paint_DrawNum(120, 20, UP, &Font24, BLACK, YELLOW);
  Paint_DrawNum(200, 120, RIGHT, &Font24, BLACK, YELLOW);
  Paint_DrawNum(120, 170, DOWN, &Font24, BLACK, YELLOW);
  Paint_DrawNum(40, 80, LEFT, &Font24, BLACK, YELLOW);
  
  Paint_DrawNum(70, 50, ALARM[0].time.TIME, &Font12, BLACK, RED);
  Paint_DrawNum(70, 60, ALARM[0].time.DATE, &Font12, BLACK, RED);
  Paint_DrawNum(70, 70, NOW.TIME, &Font12, BLACK, RED);
  Paint_DrawNum(70, 80, NOW.DATE, &Font12, BLACK, RED);

  // if the time has changed, refresh the NOW variable
  if (s != s_old) {
    setRTCTime(NOW, rtc_main);
  }

  // checking if any alarm is due at this second
  for (int i = 0; i < 5; i++) {
    if (NOW.TIME == ALARM[i].time.TIME && NOW.DATE == ALARM[i].time.DATE && ALARM[i].style != 0) {
      mode = 2;
      active_alarm = i;
    }
  }

  // // monitoring buttons -> if one is pushed, change the variable attached to that button
  // if(digitalRead(UBTN)){
  //   while(digitalRead(UBTN)){}
  //   UP = HIGH;
  // }
  // if(digitalRead(DBTN)){
  //   while(digitalRead(DBTN)){}
  //   DOWN = HIGH;
  // }
  // if(digitalRead(LBTN)){
  //   while(digitalRead(LBTN)){}
  //   LEFT = HIGH;
  // }
  // if(digitalRead(RBTN)){
  //   while(digitalRead(RBTN)){}
  //   RIGHT = HIGH;
  // }

  // changing mode, menu and submenu according to the pushed button
  if (UP || DOWN || LEFT || RIGHT) {
    refreshMenu();
  }






  ////////////////////////////////////////////////////////////////////
  // sleep mode
  ////////////////////////////////////////////////////////////////////
  if (mode == 0) {
    // if (UP != UP_old || DOWN != DOWN_old || RIGHT != RIGHT_old || LEFT != LEFT_old){
    //   mode = 1;
    //   LCD_SetBacklight(255);
    //   Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    // }
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
      //permanent submenu
      if (s_old != s && submenu == 1) {
        printDigitalTime(h, m, s);  // display the time on the screen
      }
    }

    // set alarm menu
    if (menu == 1) {
      LCD_SetBacklight(255);
      printDigitalTime(h, m, s);
    }

    // manage alarms menu
    if (menu == 2) {
      LCD_SetBacklight(255);
      printDigitalTime(h, m, s);
    }

    // settings menu
    if (menu == 3) {
      LCD_SetBacklight(255);      
      if(submenu == 0){
        Paint_DrawString_EN(60, 50, "Settings", &Font24, BLACK, WHITE);
      }
      if (submenu == 1){
        Paint_DrawString_EN(60, 50, "Clock type", &Font24, BLACK, WHITE);
      }
      if(submenu == 2){
        Paint_DrawString_EN(60, 50, "Bluetooth", &Font24, BLACK, WHITE);        
      }
    }

    // timer menu
    if (menu == 4) {
      LCD_SetBacklight(255);
      Paint_DrawString_EN(40, 50, "Set timer", &Font24, BLACK, WHITE);
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
            break;
          case 2:  // manage alarms menu
            break;
          case 3:  // settings menu
            break;
          case 4:  // timer menu
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
      /*if the device is in sleep mode*/ case 0:
        mode = 1;  // put it in active mode
        LCD_SetBacklight(255);
        Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
        break;


      /*if the device is in active mode*/ case 1:
        switch (menu) {
          /*time menu*/ case 0:
            switch (submenu) {
              /*quick watch submenu*/ case 0:
                submenu = 1;  // put it in permanent submenu
                break;
              /*permanent submenu*/ case 1:
                mode = 0;     // put it in sleep mode
                menu = 0;     // reset menu and
                submenu = 0;  // submenu
                LCD_SetBacklight(0);
                Paint_Clear(BLACK);
                break;
            }
            break;

          /*set alarm menu*/ case 1:
            switch (submenu) {
              case 0:
                submenu = 1;
                break;
              case 1:
                break;
            }
            break;
          /*manage alarms menu*/ case 2:
            switch (submenu) {
              case 0:
                submenu = 1;
                break;
              case 1:
                break;
            }
            break;
          /*settings menu*/ case 3:
            switch (submenu) {
              case 0:
                submenu = 1;
                break;
              case 1:
                break;
            }
            break;
        }


      /*if the device is in alarm mode*/ case 2:
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
      case 1:  // if the device is in active mode
        switch (menu) {
          case 0:    // time menu
            menu--;  // change menu to the left
            if (menu == -1) { menu += 5; }
            Paint_ClearWindows(27, 50, 215, 74, BLACK);
            //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
            //menu %= 5;          // put the menu value back to {0,1,2,3,4,5}, if it becomes lower than 0
            break;
          case 1:  // set alarm menu
            switch (submenu) {
              case 0:  // chose alarm submenu
                menu--;
                if (menu == -1) { menu += 5; }
                Paint_ClearWindows(27, 50, 215, 74, BLACK);
                //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
                //menu %= 5;
                break;
              case 1:
                break;
            }
            break;
          case 2:  // manage alarms menu
            switch (submenu) {
              case 0:  // step submenu
                menu--;
                if (menu == -1) { menu += 5; }
                Paint_ClearWindows(27, 50, 215, 74, BLACK);
                //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
                //menu %= 5;
                break;
              case 1:
                break;
            }
            break;
          case 3:  // settings menu
            switch (submenu) {
              case 0:  // step submenu
                menu--;
                if (menu == -1) { menu += 5; }
                Paint_ClearWindows(27, 50, 215, 74, BLACK);
                //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
                //menu %= 5;
                break;
              case 1:
                break;
            }
            break;
          case 4:  // timer menu
            switch (submenu) {
              case 0:  // step submenu
                menu--;
                if (menu == -1) { menu += 5; }
                Paint_ClearWindows(27, 50, 215, 74, BLACK);
                //Paint_DrawCircle(121, 121, 117, BLACK, DOT_PIXEL_2X2, DRAW_FILL_FULL);
                //menu %= 5;
                break;
              case 1:
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
            break;
          case 1:  // set alarm menu
            switch (submenu) {
              case 0:  // chose alarm submenu
                menu++;
                menu %= 5;
                Paint_ClearWindows(27, 50, 215, 74, BLACK);
                break;
              case 1:
                break;
            }
            break;
          case 2:  // manage alarms menu
            switch (submenu) {
              case 0:  // step submenu
                menu++;
                menu %= 5;
                Paint_ClearWindows(27, 50, 215, 74, BLACK);
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
                break;
              case 1:
                break;
            }
            break;
          case 4:  // timer menu
            switch (submenu) {
              case 0:  // step submenu
                menu++;
                menu %= 5;
                Paint_ClearWindows(27, 50, 215, 74, BLACK);
                break;
              case 1:
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




void AlarmInit(alarm input[5]) {
  for (int i = 0; i < 5; i++) {
    no = String(i + 1);
    ALARM[i].name = defname + no;
  }
}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
