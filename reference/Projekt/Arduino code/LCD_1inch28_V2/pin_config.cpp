// peripheria pin setup

#include "pin_config.h"

void pinSetup(){
  pinMode(RESONATOR, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(OFFBTN, INPUT);
  
  // pinMode(UBTN, INPUT);
  // pinMode(DBTN, INPUT);
  // pinMode(LBTN, INPUT);
  // pinMode(RBTN, INPUT);

  pinMode(UBTN, INPUT_PULLUP);
  pinMode(DBTN, INPUT_PULLUP);
  pinMode(LBTN, INPUT_PULLUP);
  pinMode(RBTN, INPUT_PULLUP);
}