// functions to print time

#ifndef _PRINT_TIME_H_
#define _PRINT_TIME_H_

#include "dates.h"
#include "GUI_Paint.h"

/*
double h, m, s;
double h_old, m_old, s_old;

int Sx, Sy, Mx, My, Hx, Hy;
int Sx_old, Sy_old, Mx_old, My_old, Hx_old, Hy_old;
double CSx, CSy, CMx, CMy, CHx, CHy;
*/
 
//void printTimeInit();
//void printAnalogTime(int h, int m, int s);
void printDigitalTime(int h, int m, int s);
void printDigitalTime(int h, int m, int s, int backcolour, int fontcolour);

#endif