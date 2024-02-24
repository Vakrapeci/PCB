// peripherial pin setup

#ifndef _PIN_CONFIG_H_
#define _PIN_CONFIG_H_

#include <stdint.h>
#include <stdio.h>
#include <SPI.h>
#include <avr/pgmspace.h>


#define RESONATOR 5
#define BUZZER    4
#define OFFBTN    12
#define UBTN      2   // interrupt pins
#define DBTN      3   // interrupt pins
#define LBTN      9   // interrupt pins
#define RBTN      10  // interrupt pins

void pinSetup();

#endif