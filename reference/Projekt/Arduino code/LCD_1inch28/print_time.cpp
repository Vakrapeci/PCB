// functions to print time

#include "print_time.h"

/*
double h, m, s;
double h_old, m_old, s_old;

int Sx, Sy, Mx, My, Hx, Hy;
int Sx_old, Sy_old, Mx_old, My_old, Hx_old, Hy_old;

double CSx, CSy, CMx, CMy, CHx, CHy;


void printTimeInit() {

  Sx_old = 0;
  Sy_old = 0;
  Mx_old = 0;
  My_old = 0;
  Hx_old = 0;
  Hy_old = 0;

  h_old = 13;
  m_old = 61;
  s_old = 61;
}

void printAnalogTime() {



  if (h != h_old) {
    CHx = 70 * sin((h / 12) * 2 * M_PI);
    CHy = 70 * cos((h / 12) * 2 * M_PI);
    Hx = 120 + CHx;
    Hy = 120 - CHy;
    Paint_DrawLine(120, 120, Hx_old, Hy_old, BLACK, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    Paint_DrawLine(120, 120, Hx, Hy, YELLOW, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    Hx_old = Hx;
    Hy_old = Hy;
    h_old = h;
  }
  
  if (m != m_old) {
    CMx = 80 * sin((m / 60) * 2 * M_PI);
    CMy = 80 * cos((m / 60) * 2 * M_PI);
    Mx = 120 + CMx;
    My = 120 - CMy;
    Paint_DrawLine(120, 120, Mx_old, My_old, BLACK, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    Paint_DrawLine(120, 120, Mx, My, BLUE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    Mx_old = Mx;
    My_old = My;
    m_old = m;
  }

  if (s != s_old) {
    CSx = 90 * sin((s / 60) * 2 * M_PI);
    CSy = 90 * cos((s / 60) * 2 * M_PI);
    Sx = 120 + CSx;
    Sy = 120 - CSy;
    Paint_DrawLine(120, 120, Sx_old, Sy_old, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    Paint_DrawLine(120, 120, Sx, Sy, RED, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    Sx_old = Sx;
    Sy_old = Sy;
    s_old = s;
  }

  if (s_old == h || m_old == h) {
    Paint_DrawLine(120, 120, Hx, Hy, YELLOW, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
  }

  if (s_old == m) {
    Paint_DrawLine(120, 120, Mx, My, BLUE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
  }
}*/

void printDigitalTime(int h, int m, int s) {

  if (h < 10) {
    Paint_DrawNum(16, 111, 0, &Font24, BLACK, GREEN);
    Paint_DrawNum(33, 111, h, &Font24, BLACK, GREEN);
  } else {
    Paint_DrawNum(16, 111, h, &Font24, BLACK, GREEN);
  }

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      Paint_SetPixel(50+i, 115+j, GREEN);
    }
  }
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      Paint_SetPixel(50+i, 123+j, GREEN);
    }
  }

  if (m < 10) {
    Paint_DrawNum(56, 111, 0, &Font24, BLACK, GREEN);
    Paint_DrawNum(73, 111, m, &Font24, BLACK, GREEN);
  } else {
    Paint_DrawNum(56, 111, m, &Font24, BLACK, GREEN);
  }

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      Paint_SetPixel(90+i, 115+j, GREEN);
    }
  }
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      Paint_SetPixel(90+i, 123+j, GREEN);
    }
  }

  if (s < 10) {
    Paint_DrawNum(96, 111, 0, &Font24, BLACK, GREEN);
    Paint_DrawNum(113, 111, s, &Font24, BLACK, GREEN);
  } else {
    Paint_DrawNum(96, 111, s, &Font24, BLACK, GREEN);
  }
}


void printDigitalTime(int h, int m, int s, int backcolour, int fontcolour) {

  if (h < 10) {
    Paint_DrawNum(16, 111, 0, &Font24, backcolour, fontcolour);
    Paint_DrawNum(33, 111, h, &Font24, backcolour, fontcolour);
  } else {
    Paint_DrawNum(16, 111, h, &Font24, backcolour, fontcolour);
  }

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      Paint_SetPixel(50+i, 115+j, fontcolour);
    }
  }
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      Paint_SetPixel(50+i, 123+j, fontcolour);
    }
  }

  if (m < 10) {
    Paint_DrawNum(56, 111, 0, &Font24, backcolour, fontcolour);
    Paint_DrawNum(73, 111, m, &Font24, backcolour, fontcolour);
  } else {
    Paint_DrawNum(56, 111, m, &Font24, backcolour, fontcolour);
  }

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      Paint_SetPixel(90+i, 115+j, fontcolour);
    }
  }
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      Paint_SetPixel(90+i, 123+j, fontcolour);
    }
  }

  if (s < 10) {
    Paint_DrawNum(96, 111, 0, &Font24, backcolour, fontcolour);
    Paint_DrawNum(113, 111, s, &Font24, backcolour, fontcolour);
  } else {
    Paint_DrawNum(96, 111, s, &Font24, backcolour, fontcolour);
  }
}