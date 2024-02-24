#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

#include <RTCZero.h>

#define UBTN 2


int h, m, s;
int s_old;
RTCZero rtc;
int state = 1;
int led = 0;
int counter = 0;


// initializing global variables
volatile int mode = 1;
volatile int menu = 0;
volatile int submenu = 0;
int mode_old = 1;

int inactiveness;

void setup() {
  //LDC setup
  Config_Init();
  LCD_Init();
  LCD_SetBacklight(255);
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
  Paint_Clear(BLACK);
  Paint_DrawCircle(120, 120, 120, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY); /*
  Paint_DrawLine  (120, 0, 120, 12,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
  Paint_DrawLine  (120, 228, 120, 240,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
  Paint_DrawLine  (0, 120, 12, 120,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
  Paint_DrawLine  (228, 120, 240, 120,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
  Paint_DrawImage (gImage_70X70, 85, 25, 70, 70); 
  Paint_DrawString_EN(123, 123, "WAVESHARE",&Font16,  BLACK, GREEN);
  Paint_DrawLine  (120, 120, 70, 70,YELLOW ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
  Paint_DrawLine  (120, 120, 176, 64,BLUE ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
  Paint_DrawLine  (120, 120, 120, 210,RED ,DOT_PIXEL_2X2,LINE_STYLE_SOLID); 

  //pinmode setup*/
  pinMode(4, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  rtc.begin();
  attachInterrupt(digitalPinToInterrupt(UBTN), ISR_UP, FALLING);
}
void loop() {



  h = rtc.getHours();
  m = rtc.getMinutes();
  s = rtc.getSeconds();
  //h = hour();
  //m = minute();
  //s = second();

  Paint_DrawNum(50, 50, h, &Font24, BLACK, RED);
  Paint_DrawNum(50, 70, m, &Font24, BLACK, RED);
  Paint_DrawNum(50, 90, s, &Font24, BLACK, RED);
  Paint_DrawNum(50, 120, state, &Font24, BLACK, RED);

  // if (state == 0) {
  //   Paint_DrawString_EN(50, 50, "Time: 1s", &Font24, BLACK, RED);
  //   if (s != s_old){
  //     digitalWrite(4, led);
  //     led = !led;
  //   }
  // }
  // if (state == 1) {
  //   Paint_DrawString_EN(50, 50, "Time: 4s", &Font24, BLACK, RED);
  //   if (s != s_old){
  //     counter++;
  //     if(counter == 4){
  //       digitalWrite(4, led);
  //       led = !led;
  //       counter = 0;
  //     }
  //   }
  // }
  if (mode == 0) {
    if (mode_old != 0){LCD_SetBacklight(0);}
    
    LCD_SetBacklight(255);
    Paint_Clear(BLACK);
  }

  if (mode == 1) {
    if (mode_old == 0){
      Paint_Clear(BLACK);
      LCD_SetBacklight(255);
      Paint_DrawCircle(121, 121, 119, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    }
    if (menu == 0) {
      // quick watch submenu
      if (s_old != s || submenu == 0) {
        //printDigitalTime(h, m, s);  // display the time on the screen
        Paint_DrawCircle(121, 121, 10, RED, DOT_PIXEL_2X2, DRAW_FILL_FULL);
        inactiveness++;
        if (inactiveness == 10) {  // if the device is inactive for 5s
          mode = 0;               // put it in sleep mode
          menu = 0;               // reset menu and
          submenu = 0;            // submanu
          inactiveness = 0;
          LCD_SetBacklight(0);  // turn off LCD
        }
      }
      //permanent submenu
      if (s_old != s || submenu == 1) {
        //printDigitalTime(h, m, s);  // display the time on the screen
        while (!digitalRead(12)){
          digitalWrite(5, led);
          led++;
          led %= 2;
          delay(100);
        }
      }
    }
  }


  delay(10);
  s_old = s;
  mode_old = mode;
}

// void ISR_UP() {
//   switch (state) {
//   case 0:
//     state = 1;
//     break;
//   case 1:
//     state = 0;
//     break;
//   }
// }

void ISR_UP() { 
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
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
